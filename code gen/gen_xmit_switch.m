function gen_xmit_switch(file_handle, ident, stage, defined_pages, array, gen_define, FPTR, slash)

	first = true;
	pages_sent = 1; %include page 0 in page count

	string_pages_to_send = [];

	switch slash
	case 1
		stages_ = {"id", "data"};
		stages = {"ids", "data"};
	case 2
		stages_ = {"cert", "sunspec"};
		stages = {"cert", "sunspec"};
	otherwise
		return
end

	define_name = [ 'NUM_', toupper(stages_{stage +1}), '_PGS2SND' ]; %toupper(ident), '_',

	string_pages_to_send = [string_pages_to_send, sprintf("uint8_t %s_pages_to_send[%s] = { 0, ", stages_{stage +1}, define_name)];

	for page = defined_pages
		if array{page}(1).stage == stage
			if first
				start_page = page;
				first = false;
			end
			pages_sent++;
			end_page = page;
			string_pages_to_send = [string_pages_to_send, sprintf("%d, ", page)];
		end
	end
	string_pages_to_send = [string_pages_to_send, sprintf("};\n")];

	start_page
	end_page
	pages_sent

	if gen_define
		fprintf(file_handle, "#define %s %d\n", define_name, pages_sent);
		fprintf(file_handle, "%s", string_pages_to_send);
		return
	end

	fprintf(file_handle, "  switch(page) {\n");
	fprintf(file_handle, "    case 0:\n");
	fprintf(file_handle, "      buffer[0] = %s;\n", [ident '_',{'id','j3072'}{slash},'_status[ch]']);
	fprintf(file_handle, "      buffer[1] = %2d; //pages to be sent\n", pages_sent);
	fprintf(file_handle, "      buffer[2] = %2d; //first (non zero) page\n", start_page);
	fprintf(file_handle, "      buffer[3] = %2d; //last page\n", end_page);
	fprintf(file_handle, "      buffer[4] = 0x00; //CRC neither sent nor read\n");
	fprintf(file_handle, "      mask = 0b0000011;\n");
	fprintf(file_handle, "      break;\n");

	string_position = 0;

	for page = defined_pages(find(defined_pages == start_page):find(defined_pages == end_page))
		fprintf(file_handle, "    case %d:\n",page);
		mask = 0b1111111;	%start assuming all bytes are unused
		last_bitfield_byte = -1;
		first_string = true;
			for index = [1:size(array{page})(2)]
				bytes_bits = array{page}(index).bytes_bits;
				if ~isempty(array{page}(index).bit_start)
					disp("bitfield")
					if last_bitfield_byte != array{page}(index).start_position
						fprintf(file_handle, "      buffer[%d] = 0xFF;\n", array{page}(index).start_position);
						last_bitfield_byte = array{page}(index).start_position;
						mask = bitset(mask, 8 - (array{page}(index).start_position + 1), 0);
					end
					fprintf(file_handle, "      buffer[%d] &= ~((~%s & 0b%s) << %d);\n", array{page}(index).start_position, ["(act_", stages{stage +1}, "_xmit[ch]->v).", array{page}(index).label], dec2bin((2^(bytes_bits)-1),8), array{page}(index).bit_start);
				else
					switch bytes_bits
						case {1, 2, 3, 4}
							fprintf("%d byte scaler\n", bytes_bits);
							for i = [1: bytes_bits]
								if i == 1
									shift_string = '';
								else
									shift_string = sprintf(" >> %d", 8*(i-1));
								end
								fprintf(file_handle, "      buffer[%d] = (%s) & 0xFF;\n", array{page}(index).start_position + i - 1, [["(act_", stages{stage +1}, "_xmit[ch]->v).", array{page}(index).label] shift_string]);
								mask = bitset(mask, 8 - (array{page}(index).start_position + i), 0);
							end
						otherwise
							disp("string/array")
							if ~first_string
								fdisp(file_handle, "      copy_buffer_data(source_ptr, dest_ptr, copy_size, false); //call copy on previous string data");
							end
							first_string = false;
							fprintf(file_handle, "      source_ptr = (uint8_t %s) %s", FPTR, ["(act_", stages{stage +1}, "_xmit[ch]->s).", array{page}(index).label]);
							if string_position ~= 0
								fprintf(file_handle, " + %d",string_position);
							end
							fprintf(file_handle, ";\n");
							if array{page}(index).start_position ~= 0
								fprintf(file_handle, "      dest_ptr = buffer + %d;\n", array{page}(index).start_position);
							end
							if array{page}(index).end_position - array{page}(index).start_position ~= 6 %end-start+1 ~= 7
								fprintf(file_handle, "      copy_size = %d;\n", array{page}(index).end_position - array{page}(index).start_position +1);
							end
							string_position += array{page}(index).end_position - array{page}(index).start_position + 1;
							if string_position == array{page}(index).bytes_bits
								string_position = 0;
							end
							string_bytes = array{page}(index).end_position - array{page}(index).start_position + 1
							mask = bitand(mask, bitcmp(bitshift((2^(string_bytes)-1), 7-string_bytes-array{page}(index).start_position),7))
						end
				end
			end
			if mask ~= 0	%not all bytes used
				fprintf(file_handle, "      mask = 0b%s;\n", dec2bin(mask,7)) %dec2bin(2^(7-(array{page}(index).end_position + 1))-1,7)
			end
		fprintf(file_handle, "      break;\n");
	end

	fprintf(file_handle, "  }\n");

end