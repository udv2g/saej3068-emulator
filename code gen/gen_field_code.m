function gen_field_code(ods_in, c_in, c_out, print_command='commented', capl=false, interactive=false)
	gen_values = strcmp(print_command, 'yes');

	[ev_numarr, ev_txtarr, ev_rawarr, ev_limits] = xlsread (ods_in, 'EvID');
	[ev_defined_pages, ev_array] = gen_page_array(ev_rawarr, 'Ev');
	[se_numarr, se_txtarr, se_rawarr, se_limits] = xlsread (ods_in, 'SeID');
	[se_defined_pages, se_array] = gen_page_array(se_rawarr, 'Se');

	[ev_defines, ev_inits] = gen_variables(ev_defined_pages, ev_array, capl)
	[se_defines, se_inits] = gen_variables(se_defined_pages, se_array, capl)

	state = 'copy';

	in_hdl = fopen(c_in, 'r')
	out_hdl = fopen(c_out, 'w')

	while ~feof(in_hdl)
		switch state
			case 'copy'
				line = fgetl(in_hdl);
				fdisp(out_hdl, line)
				if strncmp(strtrim(line), '///>',4)
					state = strtrim(line)(5:end);
				end
			case 'delete'
				line = fgetl(in_hdl);
				if strncmp(strtrim(line), '///<',4)
					fdisp(out_hdl, line)
					state = 'copy';
				end
			case 'Declarations'
				fprintf(out_hdl, "\n");
				if ~capl
					if ~isempty(ev_defines{4,1})
						fprintf(out_hdl, "%s", ev_defines{4,1}); %EV ID enumerations
						fprintf(out_hdl, "\n");
					end
					if ~isempty(ev_defines{4,2})
						fprintf(out_hdl, "%s", ev_defines{4,2}); %EV DATA enumerations
						fprintf(out_hdl, "\n");
					end
					if ~isempty(se_defines{4,1})
						fprintf(out_hdl, "%s", se_defines{4,1}); %SE ID enumerations
						fprintf(out_hdl, "\n");
					end
					if ~isempty(se_defines{4,2})
						fprintf(out_hdl, "%s", se_defines{4,2}); %SE DATA enumerations
						fprintf(out_hdl, "\n");
					end
					if ~isempty(ev_defines{1,1})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", ev_defines{1,1}); %EV ID
						fprintf(out_hdl, "} ev_ids_vars_t;\n\n");
					end
					if ~isempty(ev_defines{2,1})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", ev_defines{2,1}); %EV ID strings
						fprintf(out_hdl, "} ev_ids_strings_t;\n\n");
					end
					if ~isempty(se_defines{1,1})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", se_defines{1,1}); %SE ID
						fprintf(out_hdl, "} se_ids_vars_t;\n\n");
					end
					if ~isempty(se_defines{2,1})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", se_defines{2,1}); %SE ID strings
						fprintf(out_hdl, "} se_ids_strings_t;\n\n");
					end
					if ~isempty(ev_defines{1,2})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", ev_defines{1,2}); %EV DATA
						fprintf(out_hdl, "} ev_data_vars_t;\n\n");
					end
					if ~isempty(ev_defines{2,2})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", ev_defines{2,2}); %EV DATA strings
						fprintf(out_hdl, "} ev_data_strings_t;\n\n");
					end
					if ~isempty(se_defines{1,2})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", se_defines{1,2}); %SE DATA
						fprintf(out_hdl, "} se_data_vars_t;\n\n");
					end
					if ~isempty(se_defines{2,2})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", se_defines{2,2}); %SE DATA strings
						fprintf(out_hdl, "} se_data_strings_t;\n\n");
					end

					fprintf(out_hdl, "typedef struct  {\n");
					if ~isempty(ev_defines{2,1})
						fprintf(out_hdl, "  ev_ids_strings_t  s;\n");
					end
					if ~isempty(ev_defines{1,1})
						fprintf(out_hdl, "  ev_ids_vars_t     v;\n");
					end
					fprintf(out_hdl, "} ev_ids_t;\n\n");
					fprintf(out_hdl, "typedef struct  {\n");
					if ~isempty(ev_defines{1,2})
						fprintf(out_hdl, "  se_ids_strings_t  s;\n");
					end
					if ~isempty(se_defines{1,1})
						fprintf(out_hdl, "  se_ids_vars_t     v;\n");
					end
					fprintf(out_hdl, "} se_ids_t;\n\n");
					fprintf(out_hdl, "typedef struct  {\n");
					if ~isempty(ev_defines{2,2})
						fprintf(out_hdl, "  ev_data_strings_t s;\n");
					end
					if ~isempty(ev_defines{1,2})
						fprintf(out_hdl, "  ev_data_vars_t    v;\n");
					end
					fprintf(out_hdl, "} ev_data_t;\n\n");
					fprintf(out_hdl, "typedef struct  {\n");
					if ~isempty(se_defines{2,2})
						fprintf(out_hdl, "  se_data_strings_t s;\n");
					end
					if ~isempty(se_defines{1,2})
						fprintf(out_hdl, "  se_data_vars_t    v;\n");
					end
					fprintf(out_hdl, "} se_data_t;\n\n");

					fprintf(out_hdl, "#ifdef EV_CONFIG\n");
					fprintf(out_hdl, "  ev_ids_t ids_xmit_buff_1_A = {{\n");
					fprintf(out_hdl, "%s", ev_defines{3,1}); %EV xmit ID strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  ev_ids_t ids_xmit_buff_2_A = {{\n");
					fprintf(out_hdl, "%s", ev_defines{3,1}); %EV xmit ID strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  ev_ids_t ids_xmit_buff_1_B = {{\n");
					fprintf(out_hdl, "%s", ev_defines{3,1}); %EV xmit ID strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  ev_ids_t ids_xmit_buff_2_B = {{\n");
					fprintf(out_hdl, "%s", ev_defines{3,1}); %EV xmit ID strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  ev_data_t data_xmit_buff_1_A = {{\n");
					fprintf(out_hdl, "%s", ev_defines{3,2}); %EV xmit DATA strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  ev_data_t data_xmit_buff_2_A = {{\n");
					fprintf(out_hdl, "%s", ev_defines{3,2}); %EV xmit DATA strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  ev_data_t data_xmit_buff_1_B = {{\n");
					fprintf(out_hdl, "%s", ev_defines{3,2}); %EV xmit DATA strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  ev_data_t data_xmit_buff_2_B = {{\n");
					fprintf(out_hdl, "%s", ev_defines{3,2}); %EV xmit DATA strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  ev_ids_t *act_ids_xmit[2], *inact_ids_xmit[2];\n");
					fprintf(out_hdl, "  se_ids_t ids_rcv_buff_1_A, ids_rcv_buff_2_A, ids_rcv_buff_1_B, ids_rcv_buff_2_B, *act_ids_rcv[2], *inact_ids_rcv[2];\n");
					fprintf(out_hdl, "  ev_data_t *act_data_xmit[2], *inact_data_xmit[2];\n");
					fprintf(out_hdl, "  se_data_t data_rcv_buff_1_A, data_rcv_buff_2_A, data_rcv_buff_1_B, data_rcv_buff_2_B, *act_data_rcv[2], *inact_data_rcv[2];\n");
					fprintf(out_hdl, "#else\n");
					fprintf(out_hdl, "  se_ids_t ids_xmit_buff_1_A = {{\n");
					fprintf(out_hdl, "%s", se_defines{3,1}); %EV xmit ID strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  se_ids_t ids_xmit_buff_2_A = {{\n");
					fprintf(out_hdl, "%s", se_defines{3,1}); %EV xmit ID strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  se_ids_t ids_xmit_buff_1_B = {{\n");
					fprintf(out_hdl, "%s", se_defines{3,1}); %EV xmit ID strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  se_ids_t ids_xmit_buff_2_B = {{\n");
					fprintf(out_hdl, "%s", se_defines{3,1}); %EV xmit ID strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  se_data_t data_xmit_buff_1_A = {{\n");
					fprintf(out_hdl, "%s", se_defines{3,2}); %SE xmit DATA strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  se_data_t data_xmit_buff_2_A = {{\n");
					fprintf(out_hdl, "%s", se_defines{3,2}); %SE xmit DATA strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  se_data_t data_xmit_buff_1_B = {{\n");
					fprintf(out_hdl, "%s", se_defines{3,2}); %SE xmit DATA strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  se_data_t data_xmit_buff_2_B = {{\n");
					fprintf(out_hdl, "%s", se_defines{3,2}); %SE xmit DATA strings
					fprintf(out_hdl, "  }};\n");
					fprintf(out_hdl, "  se_ids_t *act_ids_xmit[2], *inact_ids_xmit[2];\n");
					fprintf(out_hdl, "  ev_ids_t ids_rcv_buff_1_A, ids_rcv_buff_2_A, ids_rcv_buff_1_B, ids_rcv_buff_2_B, *act_ids_rcv[2], *inact_ids_rcv[2];\n");
					fprintf(out_hdl, "  se_data_t *act_data_xmit[2], *inact_data_xmit[2];\n");
					fprintf(out_hdl, "  ev_data_t data_rcv_buff_1_A, data_rcv_buff_2_A, data_rcv_buff_1_B, data_rcv_buff_2_B, *act_data_rcv[2], *inact_data_rcv[2];\n");
					fprintf(out_hdl, "#endif\n\n");
				else
					fprintf(out_hdl, "%s", ev_defines{1,1}); %EV ID
					fprintf(out_hdl, "%s", se_defines{1,1}); %SE ID
					fprintf(out_hdl, "%s", ev_defines{1,2}); %EV DATA
					fprintf(out_hdl, "%s", se_defines{1,2}); %SE DATA
					fprintf(out_hdl, "\n");
					fprintf(out_hdl, "%s", se_defines{2,1}); %EV rcv ID strings
					fprintf(out_hdl, "%s", se_defines{2,2}); %EV rcv DATA strings
					fprintf(out_hdl, "%s", ev_defines{2,1});%SE rcv ID strings
					fprintf(out_hdl, "%s", ev_defines{2,2});%SE rcv DATA strings
				end

				state = 'delete';
			case 'pgs_to_send'
				fprintf(out_hdl, "#ifdef EV_CONFIG\n");
				gen_xmit_switch(out_hdl, 'ev', 0, ev_defined_pages, ev_array, true);
				gen_xmit_switch(out_hdl, 'ev', 1, ev_defined_pages, ev_array, true);
				fprintf(out_hdl, "#else\n");
				gen_xmit_switch(out_hdl, 'se', 0, se_defined_pages, se_array, true);
				gen_xmit_switch(out_hdl, 'se', 1, se_defined_pages, se_array, true);
				fprintf(out_hdl, "#endif\n");

				state = 'delete';
			case 'ids_init()'
				fprintf(out_hdl, "\n");
				fprintf(out_hdl, "#ifdef EV_CONFIG\n");
				fprintf(out_hdl, "void clear_ids_xmit_buff(ev_ids_t* ids_xmit_buff)  {\n");
				if ~isempty(ev_defines{1,1})
					fprintf(out_hdl, "  mem_init((ids_xmit_buff->v), (uint8_t)sizeof(ev_ids_vars_t), 0xFF);\n");
				end
				fprintf(out_hdl, "}\n");
				fprintf(out_hdl, "void clear_ids_rcv_buff(se_ids_t* ids_rcv_buff)  {\n");
				if ~isempty(se_defines{1,1})
					fprintf(out_hdl, "  mem_init(&(ids_rcv_buff->v), (uint8_t)sizeof(se_ids_vars_t), 0xFF);\n");
				end
				if ~isempty(se_defines{2,1})
					fprintf(out_hdl, "  mem_init(&(ids_rcv_buff->s), (uint8_t)sizeof(se_ids_strings_t), 0x00);\n");
				end
				fprintf(out_hdl, "}\n");
				fprintf(out_hdl, "void clear_data_xmit_buff(ev_data_t* data_xmit_buff)  {\n");
				if ~isempty(ev_defines{1,2})
					fprintf(out_hdl, "  mem_init(&(data_xmit_buff->v), (uint8_t)sizeof(ev_data_vars_t), 0xFF);\n");
				end
				if ~isempty(ev_defines{2,2})
					fprintf(out_hdl, "  mem_init(&(data_xmit_buff->s), (uint8_t)sizeof(ev_data_strings_t), 0x00);\n");
				end
				fprintf(out_hdl, "}\n");
				fprintf(out_hdl, "void clear_data_rcv_buff(se_data_t* data_rcv_buff)  {\n");
				if ~isempty(se_defines{1,2})
					fprintf(out_hdl, "  mem_init(&(data_rcv_buff->v), (uint8_t)sizeof(se_data_vars_t), 0xFF);\n");
				end
				if ~isempty(se_defines{2,2})
					fprintf(out_hdl, "  mem_init(&(data_rcv_buff->s), (uint8_t)sizeof(se_data_strings_t), 0x00);\n");
				end
				fprintf(out_hdl, "}\n");
				fprintf(out_hdl, "#else\n");
				fprintf(out_hdl, "void clear_ids_xmit_buff(se_ids_t* ids_xmit_buff)  {\n");
				if ~isempty(ev_defines{1,1})
					fprintf(out_hdl, "  mem_init((ids_xmit_buff->v), (uint8_t)sizeof(se_ids_vars_t), 0xFF);\n");
				end
				fprintf(out_hdl, "}\n");
				fprintf(out_hdl, "void clear_ids_rcv_buff(ev_ids_t* ids_rcv_buff)  {\n");
				if ~isempty(se_defines{1,1})
					fprintf(out_hdl, "  mem_init(&(ids_rcv_buff->v), (uint8_t)sizeof(ev_ids_vars_t), 0xFF);\n");
				end
				if ~isempty(se_defines{2,1})
					fprintf(out_hdl, "  mem_init(&(ids_rcv_buff->s), (uint8_t)sizeof(ev_ids_strings_t), 0x00);\n");
				end
				fprintf(out_hdl, "}\n");
				fprintf(out_hdl, "void clear_data_xmit_buff(se_data_t* data_xmit_buff)  {\n");
				if ~isempty(se_defines{1,2})
					fprintf(out_hdl, "  mem_init(&(data_xmit_buff->v), (uint8_t)sizeof(se_data_vars_t), 0xFF);\n");
				end
				if ~isempty(se_defines{2,2})
					fprintf(out_hdl, "  mem_init(&(data_xmit_buff->s), (uint8_t)sizeof(se_data_strings_t), 0x00);\n");
				end
				fprintf(out_hdl, "}\n");
				fprintf(out_hdl, "void clear_data_rcv_buff(ev_data_t* data_rcv_buff)  {\n");
				if ~isempty(ev_defines{1,2})
					fprintf(out_hdl, "  mem_init(&(data_rcv_buff->v), (uint8_t)sizeof(ev_data_vars_t), 0xFF);\n");
				end
				if ~isempty(ev_defines{2,2})
					fprintf(out_hdl, "  mem_init(&(data_rcv_buff->s), (uint8_t)sizeof(ev_data_strings_t), 0x00);\n");
				end
				fprintf(out_hdl, "}\n");
				fprintf(out_hdl, "#endif\n\n");
				fprintf(out_hdl, "\nvoid ids_init(uint8_t ch) {\n");
				fprintf(out_hdl, "  se_id_status[ch] = ID_INCOMPLETE;\n");
				fprintf(out_hdl, "  ev_id_status[ch] = ID_INCOMPLETE;\n");
				fprintf(out_hdl, "  first_data[ch]= false;\n\n");
				fprintf(out_hdl, "  act_ids_xmit[ch] = ch ? &ids_xmit_buff_1_B : &ids_xmit_buff_1_A;\n");
				fprintf(out_hdl, "  inact_ids_xmit[ch] = ch ? &ids_xmit_buff_2_B : &ids_xmit_buff_2_A;\n");
				fprintf(out_hdl, "  act_ids_rcv[ch] = ch ? &ids_rcv_buff_1_B : &ids_rcv_buff_1_A;\n");
				fprintf(out_hdl, "  inact_ids_rcv[ch] = ch ? &ids_rcv_buff_2_B : &ids_rcv_buff_2_A;\n\n");
				fprintf(out_hdl, "  act_data_xmit[ch] = ch ? &data_xmit_buff_1_B : &data_xmit_buff_1_A;\n");
				fprintf(out_hdl, "  inact_data_xmit[ch] = ch ? &data_xmit_buff_2_B : &data_xmit_buff_2_A;\n");
				fprintf(out_hdl, "  act_data_rcv[ch] = ch ? &data_rcv_buff_1_B : &data_rcv_buff_1_A;\n");
				fprintf(out_hdl, "  inact_data_rcv[ch] = ch ? &data_rcv_buff_2_B : &data_rcv_buff_2_A;\n\n");

				fprintf(out_hdl, "  clear_ids_xmit_buff(act_ids_xmit[ch]);\n");
				fprintf(out_hdl, "  clear_ids_xmit_buff(inact_ids_xmit[ch]);\n");
				fprintf(out_hdl, "  clear_ids_rcv_buff(act_ids_rcv[ch]);\n");
				fprintf(out_hdl, "  clear_ids_rcv_buff(inact_ids_rcv[ch]);\n");
				fprintf(out_hdl, "  clear_data_xmit_buff(act_data_xmit[ch]);\n");
				fprintf(out_hdl, "  clear_data_xmit_buff(inact_data_xmit[ch]);\n");
				fprintf(out_hdl, "  clear_data_rcv_buff(act_data_rcv[ch]);\n");
				fprintf(out_hdl, "  clear_data_rcv_buff(inact_data_rcv[ch]);\n\n");

				fprintf(out_hdl, "  id_xmit(ch, 0); //preload the control page\n\n");
				if gen_values
					fprintf(out_hdl, "%s\n\n", '  srand((unsigned) time(NULL));   //seed random number generator');
				end
				fprintf(out_hdl, "#ifdef EV_CONFIG\n");

				if gen_values
					fprintf(out_hdl, "  #define PREPEND (act_ids_xmit[ch]->v).\n");
					fprintf(out_hdl, "%s", ev_inits{3,1}); %EV xmit ID
					fprintf(out_hdl, "  #undef PREPEND\n");
					fprintf(out_hdl, "  #define PREPEND (act_data_xmit[ch]->v).\n");
					fprintf(out_hdl, "%s", ev_inits{3,2}); %EV xmit DATA
					fprintf(out_hdl, "  #undef PREPEND\n");
				end
				fprintf(out_hdl, "#else\n");
				if gen_values
					fprintf(out_hdl, "  #define PREPEND (act_ids_xmit[ch]->v).\n");
					fprintf(out_hdl, "%s", se_inits{3,1});%SE xmit ID
					fprintf(out_hdl, "  #undef PREPEND\n");
					fprintf(out_hdl, "  #define PREPEND (act_data_xmit[ch]->v).\n");
					fprintf(out_hdl, "%s", se_inits{3,2});%SE xmit DATA
					fprintf(out_hdl, "  #undef PREPEND\n");
				end
				fprintf(out_hdl, "#endif\n");
				if gen_values
					fprintf(out_hdl, "%s\n", '  printf("======================================================================\n");');
				end
				fprintf(out_hdl, "}\n\n");

				state = 'delete';
			case 'ev_id_parse'
				if ~capl
					gen_parse_switch(out_hdl, 'se', 0, se_defined_pages, se_array, print_command, false);	%se because ev receives se data
				else
					gen_parse_switch(out_hdl, 'ev', 0, ev_defined_pages, ev_array, print_command, true);
				end

				state = 'delete';
			case 'ev_id_xmit'
				gen_xmit_switch(out_hdl, 'ev', 0, ev_defined_pages, ev_array, false);

				state = 'delete';
			case 'ev_data_parse'
				if ~capl
					gen_parse_switch(out_hdl, 'se', 1, se_defined_pages, se_array, print_command, false);	%se because ev receives se data
				else
					gen_parse_switch(out_hdl, 'ev', 1, ev_defined_pages, ev_array, print_command, true);
				end

				state = 'delete';
			case 'ev_data_xmit'
				gen_xmit_switch(out_hdl, 'ev', 1, ev_defined_pages, ev_array, false);

				state = 'delete';

			case 'se_id_parse'
				if ~capl
					gen_parse_switch(out_hdl, 'ev', 0, ev_defined_pages, ev_array, print_command, false);	%ev because se receives ev data
				else
					gen_parse_switch(out_hdl, 'se', 0, se_defined_pages, se_array, print_command, true);
				end

				state = 'delete';
			case 'se_id_xmit'
				gen_xmit_switch(out_hdl, 'se', 0, se_defined_pages, se_array, false);

				state = 'delete';
			case 'se_data_parse'
				if ~capl
					gen_parse_switch(out_hdl, 'ev', 1, ev_defined_pages, ev_array, print_command, false);
				else
					gen_parse_switch(out_hdl, 'se', 1, se_defined_pages, se_array, print_command, true);
				end

				state = 'delete';
			case 'se_data_xmit'
				gen_xmit_switch(out_hdl, 'se', 1, se_defined_pages, se_array, false);

				state = 'delete';
			otherwise
				state
				break
		end
	end

	fclose(out_hdl);

	if interactive
		keyboard	%dbcont to continue
	end

	fclose(in_hdl);

end
