function gen_parse_switch(file_handle, ident, stage, defined_pages, array, print_command, FPTR, capl)

	if capl
		u8t = 'byte';
		u16t = 'word';
		u32t = 'dword';
		buffer_prefix = [tolower(ident), '_'];
		status_cast = '(enum ID_STATUS_TYPE)';
		switch_variable = [buffer_prefix, 'id_page_number'];
		structure = {'','','',''};
		ch_sel = '';
	else
		u8t = 'uint8_t';
		u16t = 'uint16_t';
		u32t = 'uint32_t';
		buffer_prefix = '';
		status_cast = '';
		switch_variable = sprintf('LR(ch, l_u8, %sIDPage)', [toupper(ident)(1), ident(2)]);
		structure = {'(act_ids_rcv[ch]->v).','(act_ids_rcv[ch]->s).','(act_data_rcv[ch]->v).','(act_data_rcv[ch]->s).'};	%id-v,id-s,data-v,data-s		stage*2 + (1 for v, 2 for s)
		ch_sel = '[ch]';
	end

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

	fprintf(file_handle, "  switch(%s) {\n", switch_variable);
	fprintf(file_handle, "    case 0:\n");
	fprintf(file_handle, "      pgs_to_read%s = %sbuffer[1];\n", ch_sel, buffer_prefix);
	fprintf(file_handle, "      %s_id_status%s = %s%sbuffer[0];\n",tolower(ident), ch_sel, status_cast, buffer_prefix);
	fprintf(file_handle, "      last_pg%s = %sbuffer[3];\n", ch_sel, buffer_prefix);
	fprintf(file_handle, "      pgs_read%s = 1;\n", ch_sel);
	if ~capl
		stage_string = {'ids', 'data'}{stage+1};
		fprintf(file_handle, "%s\n", ['        clear_', stage_string, '_rcv_buff(act_', stage_string, '_rcv[ch]);  //clear rcv buffer on control page to remove data from corrupted cycle']);
	else
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
		fprintf(file_handle, "%s\n", ['      write("%f-', toupper(ident), ' Status %s", timenow()/100000.0, Id_Status_strings[', tolower(ident) '_id_status]);']);
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
	fprintf(file_handle, "      break;\n");

	string_position = 0;

	for page = defined_pages(find(defined_pages == start_page):find(defined_pages == end_page))
		fprintf(file_handle, "    case %d:\n",page);
		last_bitfield_byte = -1;
		first_string = true;
		is_string = true;
			for index = [1:size(array{page})(2)]
				bytes_bits = array{page}(index).bytes_bits;
				if ~isempty(array{page}(index).bit_start)
					disp("bitfield")
					if ~capl
						temp_const_string = ['0b', dec2bin((2^(bytes_bits)-1),8)];
					else
						temp_const_string = ['0x', dec2hex((2^(bytes_bits)-1),2)];
					end
					fprintf(file_handle, "      %s = ((%sbuffer[%d] >> %d) & %s);\n", [structure{stage*2 + 1}, array{page}(index).label], buffer_prefix, array{page}(index).start_position, array{page}(index).bit_start, temp_const_string);
					temp_unit = array{page}(index).unit;
					%if strncmp(temp_unit, 'Enum',4)
					%	temp_unit = ['Enum'];
					%end
					print_rvcd_value(print_command, file_handle, array{page}(index).label, [structure{stage*2 + 1}, array{page}(index).label], array{page}(index).unit, array{page}(index).scale, array{page}(index).offset, capl, bytes_bits);
				else
					switch bytes_bits
						case {1, 2, 3, 4}
							fprintf("%d byte scaler\n", bytes_bits);
							switch bytes_bits	%fallthrough would be really nice here
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
							%	temp_unit = ['Enum'];
							%end
							print_rvcd_value(print_command, file_handle, array{page}(index).label, [structure{stage*2 + 1}, array{page}(index).label], array{page}(index).unit, array{page}(index).scale, array{page}(index).offset, capl, bytes_bits*8);
						otherwise
							disp("string/array")
							if ~first_string && ~capl
								fdisp(file_handle, "      copy_buffer_data(source_ptr, dest_ptr, copy_size, is_string); //call copy on previous string data");
							end
							first_string = false;
							if	~capl
								fprintf(file_handle, "      dest_ptr = (%s %s) %s", u8t, FPTR, [structure{stage*2 + 2}, array{page}(index).label]);
							end
							if string_position ~= 0
								if ~capl
									fprintf(file_handle, " + %d",string_position);
								end
							else
								if ~isempty(array{page}(index).unit) && array{page}(index).unit(1) == '#'		%everything about this is gross
									cur_is_string = false
								else
									cur_is_string = true
								end
							end
							if ~capl
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
							else	%is CAPL
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

	fprintf(file_handle, "  }\n");

end

function print_rvcd_value(command, file_handle, label, variable, unit, scale, offset, capl, bits)
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
	if (mod(scale,1) == 0) && (mod(offset,1) == 0)	%scale and offset are ints
		cast = '';
		format_string = '%d';
	else
		cast = '(double)';
		format_string = '%.3lf';
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
		cast = '';
		format_string = '0x%02X';
		scale_string = '';
		offset_string = '';
	end

	if strcmp(unit, '%')
		unit = '%%';
	end

	if print
		if ~capl
			if strncmp(unit, 'Enum',4)
				unit = ['Enum'];
			end
			fprintf(file_handle, "%s\n", ['      ', append, 'printf("', label, ' ', format_string, ' ', unit, '\n", (', cast, variable, scale_string, ')', offset_string, ');']);
		else
			if strcmp(unit, 'RAW')
				fprintf(file_handle, "%s\n", ['        ', append, 'write("%f-', label, ' 0x%08X", timenow()/100000.0, (', variable '));']);
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
				fprintf(file_handle, "%s\n", ['        ', append, 'write("%f-', label, ' Not_Available", timenow()/100000.0);']);
				fprintf(file_handle, "%s\n", ['      ', append, '} else if(', label, ' >= 0x', Er_min, '){']);
				fprintf(file_handle, "%s\n", ['        ', append, 'write("%f-', label, ' Error", timenow()/100000.0);']);
				if bits >= 8
					fprintf(file_handle, "%s\n", ['      ', append, '} else if(', label, ' >= 0x', Rs_min, '){']);
					fprintf(file_handle, "%s\n", ['        ', append, 'write("%f-', label, ' Reserved 0x%X", timenow()/100000.0, (', variable '));']);
				end
				fprintf(file_handle, "%s\n", ['      ', append, '} else {']);
				fprintf(file_handle, "%s\n", ['        ', append, 'write("%f-', label, ' ', format_string, ' ', unit, '", timenow()/100000.0, (', cast, variable, scale_string, ')', offset_string, ');']);
				fprintf(file_handle, "%s\n", ['      ', append, '}']);
			else	%Enums
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
		end
	end
end
