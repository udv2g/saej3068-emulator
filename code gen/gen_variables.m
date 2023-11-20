function [defines, inits] = gen_variables(defined_pages, array, capl)

	if capl
		u8t = 'byte';
		u16t = 'word';
		u32t = 'dword';
		prepend = '';
	else
		u8t = 'uint8_t';
		u16t = 'uint16_t';
		u32t = 'uint32_t';
		prepend = 'PREPEND ';
	end

	first = true;
	defines = { [], []};
	defines = [defines; defines; defines; defines];
	inits = { [], []};
	inits = [inits; inits; inits];
	last_label = [];

	for page = defined_pages
		last_bitfield_byte = -1;
			for index = [1:size(array{page})(2)]
				bytes_bits = array{page}(index).bytes_bits;
				if ~isempty(array{page}(index).bit_start)
					bytes_bits = 1;
				end

				switch bytes_bits
					case {1, 2, 3, 4}
						fprintf("%d byte scaler\n", bytes_bits);
						switch bytes_bits	%fallthrough would be really nice here
							case 1
								type = u8t;
								init_value = '0xFF';
							case 2
								type = u16t;
								init_value = '0xFFFF';
							case 3
								type = u32t;
								init_value = '0xFFFFFF';
							case 4
								type = u32t;
								init_value = '0xFFFFFFFF';
						end
						if array{page}(1).stage == -2
							defines{1, 1} = [defines{1, 1}, sprintf("  %s %s;\n", type, array{page}(index).label)];	%only define once
						else
							defines{1, array{page}(1).stage +1} = [defines{1, array{page}(1).stage +1}, sprintf("  %s %s;\n", type, array{page}(index).label)];
						end

						if ~isempty(array{page}(index).bit_start) %bitfield
							init_value = ['0b', dec2bin((2^(array{page}(index).bytes_bits)-1),8)];
						end

						if array{page}(1).stage == -2
							inits{1, 1} = [inits{1, 1}, sprintf("  %s = %s;\n", array{page}(index).label, init_value) ];
							inits{3, 1} = [inits{3, 1}, sprintf("  %s = %s;\n", [prepend, array{page}(index).label], sprintf("(%s)(rand() & %s)", type, init_value)) ];
							inits{1, 2} = [inits{1, 2}, sprintf("  %s = %s;\n", array{page}(index).label, init_value) ];
							inits{3, 2} = [inits{3, 2}, sprintf("  %s = %s;\n", [prepend, array{page}(index).label], sprintf("(%s)(rand() & %s)", type, init_value)) ];
						else
							inits{1, array{page}(1).stage +1} = [inits{1, array{page}(1).stage +1}, sprintf("  %s = %s;\n", array{page}(index).label, init_value) ];
							inits{3, array{page}(1).stage +1} = [inits{3, array{page}(1).stage +1}, sprintf("  %s = %s;\n", [prepend, array{page}(index).label], sprintf("(%s)(rand() & %s)", type, init_value)) ];
						end

						temp_unit = array{page}(index).unit;
						if strncmp(temp_unit, 'Enum',4)
							if columns(temp_unit) > 6
								temp_unit = strsplit(temp_unit(6:end),'\n');
								for i = [1:columns(temp_unit)]
									value = strtrim(strsplit(temp_unit(i){1},'=')(1){1})(1:end-1);
									switch strtrim(strsplit(temp_unit(i){1},'=')(1){1})(end)
										case 'b'
											disp('binary enum')
											value = bin2dec(value);
										case 'h'
											disp('hex enum')
											value = hex2dec(value);
										otherwise
											disp('decimal enum')
											value = str2num(strtrim(strsplit(temp_unit(i){1},'=')(1){1})(1:end));
									end
									%disp(['#define ',toupper(array{page}(index).label), '_', toupper(strrep(strtrim(strsplit(temp_unit(i){1},'=')(2){1}),' ','_')) ,' ', sprintf("0x%X",value)])

									if array{page}(1).stage == -2
										defines{4, 1} = [defines{4, 1}, ['#define ',toupper(array{page}(index).label), '_', toupper(strrep(strtrim(strsplit(temp_unit(i){1},'=')(2){1}),' ','_')) ,' ', sprintf("0x%02X\n",value) ] ];
										defines{4, 2} = [defines{4, 2}, ['#define ',toupper(array{page}(index).label), '_', toupper(strrep(strtrim(strsplit(temp_unit(i){1},'=')(2){1}),' ','_')) ,' ', sprintf("0x%02X\n",value) ] ];
									else
										defines{4, array{page}(1).stage +1} = [defines{4, array{page}(1).stage +1}, ['#define ',toupper(array{page}(index).label), '_', toupper(strrep(strtrim(strsplit(temp_unit(i){1},'=')(2){1}),' ','_')) ,' ', sprintf("0x%02X\n",value) ] ];
									end
								end
							end

							temp_unit = ['Enum'];
						end
						if array{page}(1).stage == -2
							inits{3, 1} = [inits{3, 1}, print_value(array{page}(index).label, [prepend, array{page}(index).label], temp_unit, array{page}(index).scale, array{page}(index).offset) ];
							inits{3, 2} = [inits{3, 2}, print_value(array{page}(index).label, [prepend, array{page}(index).label], temp_unit, array{page}(index).scale, array{page}(index).offset) ];
						else
							inits{3, array{page}(1).stage +1} = [inits{3, array{page}(1).stage +1}, print_value(array{page}(index).label, [prepend, array{page}(index).label], temp_unit, array{page}(index).scale, array{page}(index).offset) ];
						end
					otherwise
						disp("string/array")
						if ~strcmp(array{page}(index).label, last_label)
							%bytes_ceil = ceil(bytes_bits/7)*7;
							if capl
								temp_type = 'char';
							else
							temp_type = u8t;
							end
							if array{page}(1).stage == -2
								defines{2, 1} = [defines{2, 1}, sprintf("  %s %s[%d];\n", temp_type, array{page}(index).label,bytes_bits +1)];
							else
								defines{2, array{page}(1).stage +1} = [defines{2, array{page}(1).stage +1}, sprintf("  %s %s[%d];\n", temp_type, array{page}(index).label,bytes_bits +1)];
							end
							if ~capl
								ident_array = [];
								if array{page}(index).unit(1) ~= '#'
									source_array = array{page}(index).unit;
								else
									source_array = [];
									for i = [1:floor((length(array{page}(index).unit)-1)/2)]
										source_array = [source_array, hex2dec(array{page}(index).unit(2*i:2*i+1))];
									end
								end
								for i = 1: bytes_bits
									if i <= length(source_array)
									ident_array = [ident_array, {['''', source_array(i), ''''], sprintf("0x%02X", source_array(i))}{(array{page}(index).unit(1) == '#') +1}, ','];	% '' for single quote inside quotes
									else
									ident_array = [ident_array, '0xFF,'];
									end
								end
								ident_array = [ident_array, '''\0'''];
								if array{page}(1).stage == -2
									defines{3, 1} = [defines{3, 1}, sprintf("    {%s},\n", ident_array)];
									defines{3, 2} = [defines{3, 2}, sprintf("    {%s},\n", ident_array)];
								else
									defines{3, array{page}(1).stage +1} = [defines{3, array{page}(1).stage +1}, sprintf("    {%s},\n", ident_array)];
								end
							%else
								%defines{3, array{page}(1).stage +1} = [defines{3, array{page}(1).stage +1}, sprintf("  %s %s[] = {%s};\n", u8t, array{page}(index).label, ident_array)];
							end
							if array{page}(1).stage == -2
								inits{2, 1} = [inits{2, 1}, sprintf("  %s[0] = \'\\0\';\n", array{page}(index).label) ];
								inits{2, 2} = [inits{2, 2}, sprintf("  %s[0] = \'\\0\';\n", array{page}(index).label) ];
							else
								inits{2, array{page}(1).stage +1} = [inits{2, array{page}(1).stage +1}, sprintf("  %s[0] = \'\\0\';\n", array{page}(index).label) ];
							end
						end
						last_label = array{page}(index).label;
					end
			end
	end

	disp("===id");
	defines{1,1}
	defines{2,1}
	defines{3,1}
	inits{1,1}
	inits{2,1}
	disp("===data");
	defines{1,2}
	defines{2,2}
	defines{3,2}
	inits{1,2}
	inits{2,2}

end

function out_string = print_value(label, variable, unit, scale, offset)	%combine with print_rvcd_value() somehow?
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

	out_string = sprintf("  %s\n", ['printf("', label, ' ', format_string, ' ', unit, '\n", (', cast, variable, scale_string, ')', offset_string, ');']);
end
