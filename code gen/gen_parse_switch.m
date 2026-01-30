function gen_parse_switch(file_handle, ident, stage, defined_pages, array, print_command, FPTR, language, slash)

    switch slash
        case 1
            stages = {"ids", "data"};
        case 2
            stages = {"cert", "sunspec"};
        otherwise
            return
    end

    if strcmp(language,'CAPL')
        u8t = 'byte';
        u16t = 'word';
        u32t = 'dword';
        buffer_prefix = [tolower(ident), '_'];
        status_cast = ['(enum ',{'ID','J3072'}{slash},'_STATUS_TYPE)'];
        switch_variable = [buffer_prefix, {'id','j3072'}{slash},'_page_number'];
        structure = {'','','',''};
        ch_sel = '';
    elseif strcmp(language,'C')
        u8t = 'uint8_t';
        u16t = 'uint16_t';
        u32t = 'uint32_t';
        buffer_prefix = '';
        status_cast = '';
        switch_variable = sprintf('LR(ch, l_u8, %s%sPage)', [toupper(ident)(1), ident(2)], {'ID','J3072'}{slash});
        structure = {['(act_',stages{1},'_rcv[ch]->v).'],['(act_',stages{1},'_rcv[ch]->s).'],['(act_',stages{2},'_rcv[ch]->v).'],['(act_',stages{2},'_rcv[ch]->s).']};    %id-v,id-s,data-v,data-s        stage*2 + (1 for v, 2 for s)
        ch_sel = '[ch]';
      elseif strcmp(language,'PYTHON') % correct to my knowledge and working make changes as need be
        u8t = '';
        u16t = '';
        u32t = '';
        buffer_prefix = [tolower(ident)];
        status_cast = [{'ID','J3072'}{slash}];
        switch_variable = [buffer_prefix, {'id','j3072'}{slash},'_page_number'];
        structure = {'','','',''}; #leave blank used only for CAPL and C
        ch_sel = '';
    end

    if strcmp(language,'C') || strcmp(language,'CAPL')
        first = true;
        pages_rcvd = 1; %include page 0 in page count
        for page = defined_pages
            if (array{page}(1).stage == stage) || (array{page}(1).stage == -2)
                if first
                    start_page = page;
                    first = false;
                end
                pages_rcvd++;
                end_page = page;
            end
        end
    start_page
    end_page
    pages_rcvd

    elseif strcmp(language,'PYTHON')
        pages_to_process = [];      %fixes old page process that broke transmission for se_id stage on CAPL and Python
        for page_num = defined_pages
            if (array{page_num}(1).stage == stage) || (array{page_num}(1).stage == -2)
                pages_to_process = [pages_to_process, page_num];
            end
        end
    end



    if strcmp(language,'C') || strcmp(language,'CAPL') %takes 0 page out of python
        fprintf(file_handle, "  switch(%s) {\n", switch_variable);
        fprintf(file_handle, "    case 0:\n");
        fprintf(file_handle, "      pgs_to_read%s = %sbuffer[1];\n", ch_sel, buffer_prefix);
        fprintf(file_handle, "      %s_%s_status%s = %s%sbuffer[0];\n",tolower(ident), {"id","j3072"}{slash}, ch_sel, status_cast, buffer_prefix);
        fprintf(file_handle, "      last_pg%s = %sbuffer[3];\n", ch_sel, buffer_prefix);
        fprintf(file_handle, "      pgs_read%s = 1;\n", ch_sel);
    end

    if strcmp(language,'C')
        stage_string = stages{stage+1};
        fprintf(file_handle, "%s\n", ['        clear_', stage_string, '_rcv_buff(act_', stage_string, '_rcv[ch]);  //clear rcv buffer on control page to remove data from corrupted cycle']);
    elseif strcmp(language,'CAPL')
        if tolower(ident) == 'ev'
            xmiter = 'ev';
            rcver = 'se';
        else
            xmiter = 'se';
            rcver = 'ev';
        end
        fprintf(file_handle, "      first_pg = %sbuffer[2];\n", buffer_prefix);
        fprintf(file_handle, "      %s_crc = (%sbuffer[4] >> 4) & 0x0F;\n", xmiter, buffer_prefix);
        fprintf(file_handle, "      %s_crc = (%sbuffer[4]) & 0x0F;\n", rcver, buffer_prefix);
        fprintf(file_handle, "%s\n", ['      write("%f-', toupper(ident), ' Status %s", timenow()/100000.0, ',{'Id','J3072'}{slash},'_Status_strings[', tolower(ident) '_',{'id','j3072'}{slash},'_status]);']);
        fprintf(file_handle, "%s\n", ['      write("%f-', toupper(ident), ' Pages: First %d; Last %d; Number %d", timenow()/100000.0, first_pg, last_pg, pgs_to_read);']);
        fprintf(file_handle, "%s\n", ['      if (', xmiter, '_crc > 1) {']);
        fprintf(file_handle, "%s\n", ['        ', xmiter, '_crc = 2;']);
        fprintf(file_handle, "%s\n", ['        }']);
        fprintf(file_handle, "%s\n", ['        if (',  rcver, '_crc > 1) {']);
        fprintf(file_handle, "%s\n", ['          if (',  rcver, '_crc < 0xE)  {']);
        fprintf(file_handle, "%s\n", ['            ',  rcver, '_crc = 4; //Reserved']);
        fprintf(file_handle, "%s\n", ['          } else  {']);
        fprintf(file_handle, "%s\n", ['            ',  rcver, '_crc = ',  rcver, '_crc - 12;']);
        fprintf(file_handle, "%s\n", ['          }']);
        fprintf(file_handle, "%s\n", ['        }']);
        fprintf(file_handle, "%s\n", ['      write("%f-', toupper(xmiter), ' CRC Stat: %s; ', toupper(rcver),' CRC Stat: %s", timenow()/100000.0, xmit_crc_status[', xmiter, '_crc], rcv_crc_status[', rcver, '_crc]);']);
    end

    if strcmp(language,'C') || strcmp(language,'CAPL') %takes 0 page out of python
        fprintf(file_handle, "      break;\n");
    end

    string_position = 0;
    if strcmp(language,'C') || strcmp(language,'CAPL')
        for page = defined_pages(find(defined_pages == start_page):find(defined_pages == end_page))
            fprintf(file_handle, "    case %d:\n",page);
            last_bitfield_byte = -1;
            first_string = true;
            is_string = true;
                for index = [1:size(array{page})(2)]
                    bytes_bits = array{page}(index).bytes_bits;
                    if ~isempty(array{page}(index).bit_start)
                        disp("bitfield")
                        if strcmp(language,'C')
                            temp_const_string = ['0b', dec2bin((2^(bytes_bits)-1),8)];
                        elseif strcmp(language,'CAPL')
                            temp_const_string = ['0x', dec2hex((2^(bytes_bits)-1),2)];
                        elseif strcmp(language,'PYTHON')
                            temp_const_string = ['0x', dec2hex((2^(bytes_bits)-1),2)];
                        end
                        fprintf(file_handle, "             %s = ((%sbuffer[%d] >> %d) & %s);\n", [structure{stage*2 + 1}, array{page}(index).label], buffer_prefix, array{page}(index).start_position, array{page}(index).bit_start, temp_const_string);
                        temp_unit = array{page}(index).unit;
                        %if strncmp(temp_unit, 'Enum',4)
                        %    temp_unit = ['Enum'];
                        %end


                        print_rvcd_value(print_command, file_handle, array{page}(index).label, [structure{stage*2 + 1}, array{page}(index).label], array{page}(index).unit, array{page}(index).scale, array{page}(index).offset, language, bytes_bits,page);

                    else
                        switch bytes_bits
                            case {1, 2, 3, 4}
                                fprintf("%d byte scaler\n", bytes_bits);
                                switch bytes_bits    %fallthrough would be really nice here
                                    case 1
                                        type = u8t;
                                    case 2
                                        type = u16t;
                                    case {3, 4}
                                        type = u32t;
                                end
                                fprintf(file_handle, "      %s =",[structure{stage*2 + 1}, array{page}(index).label]);
                                for i = [bytes_bits : -1 : 1]
                                    if i == 1
                                        shift_string = '';
                                    else
                                        shift_string = sprintf(" << %d", 8*(i-1));
                                    end
                                    if i == bytes_bits
                                        or_string = '';
                                    else
                                        or_string = ' |';
                                    end
                                    fprintf(file_handle, "%s ((%s)%sbuffer[%d]%s)", or_string, type, buffer_prefix, array{page}(index).start_position + i - 1, shift_string);
                                end
                                fprintf(file_handle, ";\n");
                                %temp_unit = array{page}(index).unit;
                                %if strncmp(temp_unit, 'Enum',4)
                                %    temp_unit = ['Enum'];
                                %end

                                print_rvcd_value(print_command, file_handle, array{page}(index).label, [structure{stage*2 + 1}, array{page}(index).label], array{page}(index).unit, array{page}(index).scale, array{page}(index).offset, language, bytes_bits*8,page);

                            otherwise
                                disp("string/array")
                                if ~first_string && strcmp(language,'C')
                                    fdisp(file_handle, "      copy_buffer_data(source_ptr, dest_ptr, copy_size, is_string); //call copy on previous string data");
                                end
                                first_string = false;
                                if    strcmp(language,'C')
                                    fprintf(file_handle, "      dest_ptr = (%s %s) %s", u8t, FPTR, [structure{stage*2 + 2}, array{page}(index).label]);
                                end
                                if string_position ~= 0
                                    if strcmp(language,'C')
                                        fprintf(file_handle, " + %d",string_position);
                                    end
                                else
                                    if ~isempty(array{page}(index).unit) && array{page}(index).unit(1) == '#'        %everything about this is gross
                                        cur_is_string = false
                                    else
                                        cur_is_string = true
                                    end
                                end
                                if strcmp(language,'C')
                                    fprintf(file_handle, ";\n");
                                    if array{page}(index).start_position ~= 0
                                        fprintf(file_handle, "      source_ptr = buffer + %d;\n", array{page}(index).start_position);
                                    end
                                    if array{page}(index).end_position - array{page}(index).start_position ~= 6 %end-start+1 ~= 7
                                        fprintf(file_handle, "      copy_size = %d;\n", array{page}(index).end_position - array{page}(index).start_position +1);
                                    end
                                    if is_string ~= cur_is_string
                                        fprintf(file_handle, "      is_string = %s;\n", { 'false', 'true'}{cur_is_string +1});
                                    end
                                elseif strcmp(language,'CAPL')    %is CAPL
                                    fprintf(file_handle, "      copy_page(%sbuffer, %s, %d, %d, %d, %s);\n", buffer_prefix, array{page}(index).label, array{page}(index).start_position, string_position, array{page}(index).end_position - array{page}(index).start_position +1, { 'false', 'true'}{cur_is_string +1});
                                    end
                                is_string = cur_is_string;
                                string_position += array{page}(index).end_position - array{page}(index).start_position + 1;
                                if string_position == array{page}(index).bytes_bits
                                    string_position = 0;
                                end
                                string_bytes = array{page}(index).end_position - array{page}(index).start_position + 1
                            end
                    end
                end
            fprintf(file_handle, "      break;\n");
        end
    elseif strcmp(language,'PYTHON')
        for page = pages_to_process



            fprintf(file_handle, "                elif page == %d:\n",page);


            last_bitfield_byte = -1;
            first_string = true;
            is_string = true;
                for index = [1:size(array{page})(2)]
                    bytes_bits = array{page}(index).bytes_bits;

                    if ~isempty(array{page}(index).bit_start)
                        disp("bitfield")
                        if strcmp(language,'C')
                            temp_const_string = ['0b', dec2bin((2^(bytes_bits)-1),8)];
                        elseif strcmp(language,'CAPL')
                            temp_const_string = ['0x', dec2hex((2^(bytes_bits)-1),2)];
                        elseif strcmp(language,'PYTHON')
                            temp_const_string = ['0x', dec2hex((2^(bytes_bits)-1),2)];
                        end

                        fprintf(file_handle, "                    %s = (((databytes[%d] >> %d) & %s))\n", [structure{stage*2 + 1}, array{page}(index).label], array{page}(index).start_position + 1, array{page}(index).bit_start, temp_const_string);
                        temp_unit = array{page}(index).unit;
                        %if strncmp(temp_unit, 'Enum',4)
                        %    temp_unit = ['Enum'];
                        %end

                        print_rvcd_value(print_command, file_handle, array{page}(index).label, [structure{stage*2 + 1}, array{page}(index).label], array{page}(index).unit, array{page}(index).scale, array{page}(index).offset, language, bytes_bits,page);


                    else
                        switch bytes_bits
                            case {1, 2, 3, 4}
                                fprintf("%d byte scaler\n", bytes_bits);
                                switch bytes_bits    %fallthrough would be really nice here
                                    case 1
                                        type = u8t;
                                    case 2
                                        type = u16t;
                                    case {3, 4}
                                        type = u32t;
                                end


                                fprintf(file_handle, "                    %s =",[structure{stage*2 + 1}, array{page}(index).label]);



                                for i = [bytes_bits : -1 : 1]
                                    if i == 1
                                        shift_string = '';
                                    else
                                        shift_string = sprintf(" << %d", 8*(i-1));
                                    end
                                    if i == bytes_bits
                                        or_string = '';
                                    else
                                        or_string = ' |';
                                    end
                                    if strcmp(language,'C') || strcmp(language,'CAPL')
                                        fprintf(file_handle, "%s ((%s)%sbuffer[%d]%s)", or_string, type, buffer_prefix, array{page}(index).start_position + i - 1, shift_string);
                                    elseif strcmp(language,'PYTHON')


                                          fprintf(file_handle, "%s (%sdatabytes[%d]%s)", or_string, type, array{page}(index).start_position + i , shift_string);


                                    end

                                end  % end of function that controls this SeAmbientTemp = (()se_buffer[1] << 8) | (()se_buffer[0])
                                if strcmp(language,'C') || strcmp(language,'CAPL')
                                    fprintf(file_handle, ";\n");
                                elseif strcmp(language,'PYTHON')
                                    fprintf(file_handle, "\n");
                                end

                                %temp_unit = array{page}(index).unit;
                                %if strncmp(temp_unit, 'Enum',4)
                                %    temp_unit = ['Enum'];
                                %end

                                print_rvcd_value(print_command, file_handle, array{page}(index).label, [structure{stage*2 + 1}, array{page}(index).label], array{page}(index).unit, array{page}(index).scale, array{page}(index).offset, language, bytes_bits*8 ,page);

                            otherwise
                                disp("string/array")
                                if ~first_string && strcmp(language,'C')
                                    fdisp(file_handle, "      copy_buffer_data(source_ptr, dest_ptr, copy_size, is_string); //call copy on previous string data");
                                end
                                first_string = false;
                                if    strcmp(language,'C')
                                    fprintf(file_handle, "                  dest_ptr = (%s %s) %s", u8t, FPTR, [structure{stage*2 + 2}, array{page}(index).label]);
                                end
                                if string_position ~= 0
                                    if strcmp(language,'C')
                                        fprintf(file_handle, " + %d",string_position);
                                    end
                                else
                                    if ~isempty(array{page}(index).unit) && array{page}(index).unit(1) == '#'        %everything about this is gross
                                        cur_is_string = false
                                    else
                                        cur_is_string = true
                                    end
                                end
                                if strcmp(language,'C')
                                    fprintf(file_handle, ";\n");
                                    if array{page}(index).start_position ~= 0
                                        fprintf(file_handle, "      source_ptr = buffer + %d;\n", array{page}(index).start_position);
                                    end
                                    if array{page}(index).end_position - array{page}(index).start_position ~= 6 %end-start+1 ~= 7
                                        fprintf(file_handle, "      copy_size = %d;\n", array{page}(index).end_position - array{page}(index).start_position +1);
                                    end
                                    if is_string ~= cur_is_string
                                        fprintf(file_handle, "      is_string = %s;\n", { 'false', 'true'}{cur_is_string +1});
                                    end
                                elseif strcmp(language,'CAPL')     %is CAPL
                                    fprintf(file_handle, "      copy_page(%sbuffer, %s, %d, %d, %d, %s);\n", buffer_prefix, array{page}(index).label, array{page}(index).start_position, string_position, array{page}(index).end_position - array{page}(index).start_position +1, { 'false', 'true'}{cur_is_string +1});

                                elseif strcmp(language, 'PYTHON')  %is PYTHON

                                    fprintf(file_handle, '                    result_display.append(f"    %s Page: {page} (Buffering Data)")\n', array{page}(index).label);

                                    fprintf(file_handle, '                    process_stage(%s%s_buffer, "%s", string_position = %d,  data_bytes = databytes[1:%d], field_total_length = %d)\n\n', buffer_prefix, status_cast, array{page}(index).label, string_position, array{page}(index).end_position - array{page}(index).start_position +2, array{page}(index).bytes_bits);               end
                                is_string = cur_is_string;
                                string_position += array{page}(index).end_position - array{page}(index).start_position + 1;
                                if string_position == array{page}(index).bytes_bits
                                    string_position = 0;
                                end
                                string_bytes = array{page}(index).end_position - array{page}(index).start_position + 1
                            end
                    end
                end

        end
    end

    if strcmp(language,'C') || strcmp(language,'CAPL')
        fprintf(file_handle, "  }\n");
    end

end

function print_rvcd_value(command, file_handle, label, variable, unit, scale, offset, language, bits, page)
    print = true;
    append = '';

    switch command
        case 'no'
            print = false;
        case 'yes'
        case 'commented'
            append = '//';
        otherwise
            disp('Unexpected option in print_rvcd_value()');
    end
    if (mod(scale,1) == 0) && (mod(offset,1) == 0)    %scale and offset are ints
        if strcmp(language,'C') || strcmp(language,'CAPL')
            cast = '';
            format_string = '%d';
        elseif strcmp(language, 'PYTHON')
            cast = '';
            format_string = '';
        end
    else
        if strcmp(language,'C') || strcmp(language,'CAPL')
            cast = '(double)';
            format_string = '%.3lf';
        elseif strcmp(language, 'PYTHON')
            cast = '';
            format_string = ':.3f';
        end
    end
    if scale == 1
        scale_string = '';
    elseif mod(scale,1) == 0 %is an integer
        scale_string = sprintf(" * %d", scale);
    else
        scale_string = sprintf(" * %f", scale);
    end
    if offset == 0
        offset_string = '';
    elseif mod(offset,1) == 0 %is an integer
        offset_string = sprintf(" - %d", -offset);
    else
        offset_string = sprintf(" - %f", -offset);
    end
    if isempty(scale) || isempty(offset) %raw data
        if strcmp(language,'C') || strcmp(language,'CAPL')
            cast = '';
            format_string = '0x%02X';
            scale_string = '';
            offset_string = '';
        elseif strcmp(language, 'PYTHON')
            cast = '0x';
            format_string = ':02X';
            scale_string = '';
            offset_string = '';
        end
    end

    if strcmp(unit, '%')
        if strcmp(language,'C') || strcmp(language,'CAPL')
            unit = '%%';
        elseif strcmp(language, 'PYTHON')
            unit = '%';
        end
    end

    if print
        if strcmp(language,'C')
            if strncmp(unit, 'Enum',4)
                unit = ['Enum'];
            end
            fprintf(file_handle, "%s\n", ['      ', append, 'printf("', label, ' ', format_string, ' ', unit, '\n", (', cast, variable, scale_string, ')', offset_string, ');']);
        elseif strcmp(language,'CAPL')
            if strcmp(unit, 'RAW')
                fprintf(file_handle, "%s\n", ['                ', append, 'write("%f-', label, ' 0x%08X", timenow()/100000.0, (', variable '));']);
            elseif ~strncmp(unit, 'Enum',4)
                if mod(bits,8) == 0
                    zero_string = [];
                    for i = [1:(bits/8-1)]
                        zero_string = [zero_string, '00'];
                    end
                    NA_min = ['FF', zero_string];
                    Er_min = ['FE', zero_string];
                    Rs_min = ['FB', zero_string];
                else
                    NA_min = dec2hex((2^(bits)-1));
                    Er_min = dec2hex((2^(bits)-2));
                    if bits >= 8
                        Rs_min = dec2hex((2^(bits)-5));
                    end
                end
                fprintf(file_handle, "%s\n", ['      ', append, 'if(', label, ' >= 0x', NA_min, '){']);
                fprintf(file_handle, "%s\n", ['        ', append, 'write("%f-', label, ' N/A", timenow()/100000.0);']);
                fprintf(file_handle, "%s\n", ['      ', append, '} else if(', label, ' >= 0x', Er_min, '){']);
                fprintf(file_handle, "%s\n", ['        ', append, 'write("%f-', label, ' Error", timenow()/100000.0);']);
                if bits >= 8
                    fprintf(file_handle, "%s\n", ['      ', append, '} else if(', label, ' >= 0x', Rs_min, '){']);
                    fprintf(file_handle, "%s\n", ['        ', append, 'write("%f-', label, ' Reserved 0x%X", timenow()/100000.0, (', variable '));']);
                end
                fprintf(file_handle, "%s\n", ['      ', append, '} else {']);
                fprintf(file_handle, "%s\n", ['        ', append, 'write("%f-', label, ' ', format_string, ' ', unit, '", timenow()/100000.0, (', cast, variable, scale_string, ')', offset_string, ');']);
                fprintf(file_handle, "%s\n", ['      ', append, '}']);
            else    %Enums
                if columns(unit) > 6
                    fprintf(file_handle, "%s\n", ['      ', append, 'switch(', variable, ')  {']);
                    unit = strsplit(unit(6:end),'\n');
                    for i = [1:columns(unit)]
                        value = strtrim(strsplit(unit(i){1},'=')(1){1})(1:end-1);
                        switch strtrim(strsplit(unit(i){1},'=')(1){1})(end)
                            case 'b'
                                disp('binary enum')
                                value = bin2dec(value);
                            case 'h'
                                disp('hex enum')
                                value = hex2dec(value);
                            otherwise
                                disp('decimal enum')
                                value = str2num(strtrim(strsplit(unit(i){1},'=')(1){1})(1:end));
                        end
                        fprintf(file_handle, "%s\n", ['      ', append, '  case ', sprintf("0x%02X",value), ':']);
                        fprintf(file_handle, "%s\n", ['      ', append, '    write("%f-', label, ' ', strrep(strtrim(strsplit(unit(i){1},'=')(2){1}),' ','_'), '", timenow()/100000.0, (', cast, variable, scale_string, ')', offset_string, ');']);
                        fprintf(file_handle, "%s\n", ['      ', append, '    break;']);
                    end
                    fprintf(file_handle, "%s\n", ['      ', append, '  default:']);
                    fprintf(file_handle, "%s\n", ['      ', append, '    write("%f-', label, ' Reserved ', format_string, ' ', '", timenow()/100000.0, ', variable, ');']);
                    fprintf(file_handle, "%s\n", ['      ', append, '}']);
                else
                    fprintf(file_handle, "%s\n", ['      ', append, 'write("%f-', label, ' ', format_string, 'Enum', '", timenow()/100000.0, ', variable, ');']);
                end
            end
        elseif strcmp(language,'PYTHON') %Reconfigure the below for the formatting the output file for Python


            fprintf(file_handle,'                    result_display.append(f"   %s Page: %d")\n',variable,page);

            if strcmp(unit, 'RAW')  %fix this, CRC page 251


                    fprintf(file_handle, '                    result_display.append(f"   %s from frame = 0x{%s:08X}")\n', variable, variable);
                    fprintf(file_handle, "%s\n", ['                    ', append, 'is_match, computed_crc = crc_check( ', variable, ', ', tolower(variable(1:2)),'ID_stage_bytes)']); 
                    fprintf(file_handle, '                    if is_match:\n')
                    fprintf(file_handle, '                        result_display.append(f"    <b><span style=''color:green;''>    &nbsp;&nbsp;&nbsp;CRC Check: MATCH</span></b> (Computed: 0x{computed_crc:08X})")\n');
                    fprintf(file_handle, '                    else:\n')
                    fprintf(file_handle, '                        result_display.append(f"    <b><span style=''color:red;''>    &nbsp;&nbsp;&nbsp;CRC Check: MISMATCH</span></b> (Computed: 0x{computed_crc:08X})")\n\n');
                    fprintf(file_handle, '                    %sID_stage_bytes.clear()\n', tolower(variable(1:2)))


            elseif ~strncmp(unit, 'Enum',4)
                if mod(bits,8) == 0
                    zero_string = [];
                    for i = [1:(bits/8-1)]
                        zero_string = [zero_string, '00'];
                    end
                    NA_min = ['FF', zero_string];
                    Er_min = ['FE', zero_string];
                    Rs_min = ['FB', zero_string];
                else
                    NA_min = dec2hex((2^(bits)-1));
                    Er_min = dec2hex((2^(bits)-2));
                    if bits >= 8
                        Rs_min = dec2hex((2^(bits)-5));
                    end
                end

                %here is where se data populates
                fprintf(file_handle, "%s\n", ['                    ', append, 'if ', label, ' >= 0x', NA_min, ':']);
                fprintf(file_handle, "%s\n", ['                        ', append, 'update_op_data("' label, '",',  ' ', 'f"', 'N/A', '"', ')']);
                fprintf(file_handle, "%s\n", ['                    ', append, 'elif ', label, ' >= 0x', NA_min, ':']);
                fprintf(file_handle, "%s\n", ['                        ', append, 'update_op_data("' label, '",',  ' ', 'f"', 'Error', '"', ')']);
                if bits >= 8
                    fprintf(file_handle, "%s\n", ['                    ', append, 'elif ', label, ' >= 0x', Rs_min, ':']);
                    fprintf(file_handle, "%s\n", ['                        ', append, 'update_op_data(', '"', variable, '"', ', f"Reserved 0x',  '{',variable, ':02X}', '")']);
                end
                fprintf(file_handle, "%s\n", ['                    ', append, 'else', ':']);
                fprintf(file_handle, "%s\n", ['                        ', append, 'update_op_data("' label, '",',  ' ', 'f"', cast,'{', '(', variable, scale_string, ')', offset_string, format_string, '} ', unit , '"', ')']);
                fprintf(file_handle, "\n");


            else    %Enums not formated yet
                fprintf(file_handle, "\n");

                if columns(unit) > 6
                    fprintf(file_handle, "%s\n", ['                    ', append, 'match ', variable, ':']);

                    unit = strsplit(unit(6:end),'\n');
                    for i = [1:columns(unit)]
                        value = strtrim(strsplit(unit(i){1},'=')(1){1})(1:end-1);
                        switch strtrim(strsplit(unit(i){1},'=')(1){1})(end)
                            case 'b'
                                disp('binary enum')
                                value = bin2dec(value);

                            case 'h'
                                disp('hex enum')
                                value = hex2dec(value);

                            otherwise
                                disp('decimal enum')
                                value = str2num(strtrim(strsplit(unit(i){1},'=')(1){1})(1:end));

                        end
                        fprintf(file_handle, "%s\n", ['                        ', append, 'case ', sprintf("0x%02X",value), ':']);   %This is where formatting starts and goes down
                        % line below unused I think
                        fprintf(file_handle, "%s\n", ['                            ', append, 'update_op_data(', '"', variable, '"',', "' strrep(strtrim(strsplit(unit(i){1},'=')(2){1}),' ','_'), '"', scale_string, offset_string, ')']);
                    end
                    fprintf(file_handle, "%s\n", ['                        ', append, 'case _:']);
                    fprintf(file_handle, "%s\n", ['                            ', append, 'update_op_data(', '"', variable, '"', ', f"Reserved 0x',  '{',variable, ':02X}', '")']);
                    fprintf(file_handle, "\n");
                    fprintf(file_handle, "");

                else
                    %fprintf(file_handle, "%s\n", ['      ', append, 'update_op_data("', ' ', format_string, 'Enum', ', (', variable, '))']);
                end
            end
        end
    end
end
