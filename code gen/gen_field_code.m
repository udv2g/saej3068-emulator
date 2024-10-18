function gen_field_code(slash, ods_in, c_in, c_out, print_command='commented', rpage=false, capl=false, interactive=false)
	gen_values = strcmp(print_command, 'yes');

	no_stage_2 = false;

	switch slash
		case 1
			evsheet = 'EvID';
			sesheet = 'SeID';
			stages = {"ids", "data"};
		case 2
			evsheet = 'EvJ3072';
			sesheet = 'SeJ3072';
			stages = {"cert", "sunspec"};
			no_stage_2 = true;
		otherwise
			return
	end

	if rpage
		FPTR = "* far ";
	else
		FPTR = "*";
	end

	[ev_numarr, ev_txtarr, ev_rawarr, ev_limits] = xlsread (ods_in, evsheet);
	[ev_defined_pages, ev_array] = gen_page_array(ev_rawarr, 'Ev');
	[se_numarr, se_txtarr, se_rawarr, se_limits] = xlsread (ods_in, sesheet);
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
						fprintf(out_hdl, ["} ev_",stages{1},"_vars_t;\n\n"]);
					end
					if ~isempty(ev_defines{2,1})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", ev_defines{2,1}); %EV ID strings
						fprintf(out_hdl, ["} ev_",stages{1},"_strings_t;\n\n"]);
					end
					if ~isempty(se_defines{1,1})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", se_defines{1,1}); %SE ID
						fprintf(out_hdl, ["} se_",stages{1},"_vars_t;\n\n"]);
					end
					if ~isempty(se_defines{2,1})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", se_defines{2,1}); %SE ID strings
						fprintf(out_hdl, ["} se_",stages{1},"_strings_t;\n\n"]);
					end
					if ~isempty(ev_defines{1,2})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", ev_defines{1,2}); %EV DATA
						fprintf(out_hdl, ["} ev_",stages{2},"_vars_t;\n\n"]);
					end
					if ~isempty(ev_defines{2,2})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", ev_defines{2,2}); %EV DATA strings
						fprintf(out_hdl, ["} ev_",stages{2},"_strings_t;\n\n"]);
					end
					if ~isempty(se_defines{1,2})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", se_defines{1,2}); %SE DATA
						fprintf(out_hdl, ["} se_",stages{2},"_vars_t;\n\n"]);
					end
					if ~isempty(se_defines{2,2})
						fprintf(out_hdl, "typedef struct  {\n");
						fprintf(out_hdl, "%s", se_defines{2,2}); %SE DATA strings
						fprintf(out_hdl, ["} se_",stages{2},"_strings_t;\n\n"]);
					end

					fprintf(out_hdl, "typedef struct  {\n");
					if ~isempty(ev_defines{2,1})
						fprintf(out_hdl, ["  ev_",stages{1},"_strings_t  s;\n"]);
					end
					if ~isempty(ev_defines{1,1})
						fprintf(out_hdl, ["  ev_",stages{1},"_vars_t     v;\n"]);
					end
					fprintf(out_hdl, ["} ev_",stages{1},"_t;\n\n"]);
					fprintf(out_hdl, "typedef struct  {\n");
					if ~isempty(se_defines{2,1})
						fprintf(out_hdl, ["  se_",stages{1},"_strings_t  s;\n"]);
					end
					if ~isempty(se_defines{1,1})
						fprintf(out_hdl, ["  se_",stages{1},"_vars_t     v;\n"]);
					end
					fprintf(out_hdl, ["} se_",stages{1},"_t;\n\n"]);
					if ~no_stage_2
						fprintf(out_hdl, "typedef struct  {\n");
						if ~isempty(ev_defines{2,2})
							fprintf(out_hdl, ["  ev_",stages{2},"_strings_t s;\n"]);
						end
						if ~isempty(ev_defines{1,2})
							fprintf(out_hdl, ["  ev_",stages{2},"_vars_t    v;\n"]);
						end
						fprintf(out_hdl, ["} ev_",stages{2},"_t;\n\n"]);
						fprintf(out_hdl, "typedef struct  {\n");
						if ~isempty(se_defines{2,2})
							fprintf(out_hdl, ["  se_",stages{2},"_strings_t s;\n"]);
						end
						if ~isempty(se_defines{1,2})
							fprintf(out_hdl, ["  se_",stages{2},"_vars_t    v;\n"]);
						end
						fprintf(out_hdl, ["} se_",stages{2},"_t;\n\n"]);
					end

					fprintf(out_hdl, "#ifdef EV_CONFIG\n");
					if rpage
						fprintf(out_hdl, "#pragma DATA_SEG __RPAGE_SEG PAGED_RAM\n");
					end
					fprintf(out_hdl, ["  ev_",stages{1},"_t ",stages{1},"_xmit_buff_1_A"]);
					if ~isempty(ev_defines{3,1})
						fprintf(out_hdl, " = {{\n");
						fprintf(out_hdl, "%s", ev_defines{3,1}); %EV xmit ID strings
						fprintf(out_hdl, "  }};\n");
					else
						fprintf(out_hdl, ";\n");
					end
					fprintf(out_hdl, ["  ev_",stages{1},"_t ",stages{1},"_xmit_buff_2_A"]);
					if ~isempty(ev_defines{3,1})
						fprintf(out_hdl, " = {{\n");
						fprintf(out_hdl, "%s", ev_defines{3,1}); %EV xmit ID strings
						fprintf(out_hdl, "  }};\n");
					else
						fprintf(out_hdl, ";\n");
					end
					fprintf(out_hdl, ["  ev_",stages{1},"_t ",stages{1},"_xmit_buff_1_B"]);
					if ~isempty(ev_defines{3,1})
						fprintf(out_hdl, " = {{\n");
						fprintf(out_hdl, "%s", ev_defines{3,1}); %EV xmit ID strings
						fprintf(out_hdl, "  }};\n");
					else
						fprintf(out_hdl, ";\n");
					end
					fprintf(out_hdl, ["  ev_",stages{1},"_t ",stages{1},"_xmit_buff_2_B"]);
					if ~isempty(ev_defines{3,1})
						fprintf(out_hdl, " = {{\n");
						fprintf(out_hdl, "%s", ev_defines{3,1}); %EV xmit ID strings
						fprintf(out_hdl, "  }};\n");
					else
						fprintf(out_hdl, ";\n");
					end
					if ~no_stage_2
						fprintf(out_hdl, ["  ev_",stages{2},"_t ",stages{2},"_xmit_buff_1_A"]);
						if ~isempty(ev_defines{3,2})
							fprintf(out_hdl, " = {{\n");
							fprintf(out_hdl, "%s", ev_defines{3,2}); %EV xmit DATA strings
							fprintf(out_hdl, "  }};\n");
						else
							fprintf(out_hdl, ";\n");
						end
						fprintf(out_hdl, ["  ev_",stages{2},"_t ",stages{2},"_xmit_buff_2_A"]);
						if ~isempty(ev_defines{3,2})
							fprintf(out_hdl, " = {{\n");
							fprintf(out_hdl, "%s", ev_defines{3,2}); %EV xmit DATA strings
							fprintf(out_hdl, "  }};\n");
						else
							fprintf(out_hdl, ";\n");
						end
						fprintf(out_hdl, ["  ev_",stages{2},"_t ",stages{2},"_xmit_buff_1_B"]);
						if ~isempty(ev_defines{3,2})
							fprintf(out_hdl, " = {{\n");
							fprintf(out_hdl, "%s", ev_defines{3,2}); %EV xmit DATA strings
							fprintf(out_hdl, "  }};\n");
						else
							fprintf(out_hdl, ";\n");
						end
						fprintf(out_hdl, ["  ev_",stages{2},"_t ",stages{2},"_xmit_buff_2_B"]);
						if ~isempty(ev_defines{3,2})
							fprintf(out_hdl, " = {{\n");
							fprintf(out_hdl, "%s", ev_defines{3,2}); %EV xmit DATA strings
							fprintf(out_hdl, "  }};\n");
						else
							fprintf(out_hdl, ";\n");
						end
					end
					if ~no_stage_2
						fprintf(out_hdl, ["  se_",stages{2},"_t ",stages{2},"_rcv_buff_1_A, ",stages{2},"_rcv_buff_2_A, ",stages{2},"_rcv_buff_1_B, ",stages{2},"_rcv_buff_2_B;\n"]);
					end
					fprintf(out_hdl, ["  se_",stages{1},"_t ",stages{1},"_rcv_buff_1_A, ",stages{1},"_rcv_buff_2_A, ",stages{1},"_rcv_buff_1_B, ",stages{1},"_rcv_buff_2_B;\n"]);
					if rpage
						fprintf(out_hdl, "#pragma DATA_SEG DEFAULT\n\n");
					end
					fprintf(out_hdl, ["  ev_",stages{1},"_t ", FPTR, "act_",stages{1},"_xmit[2], ", FPTR, "inact_",stages{1},"_xmit[2];\n"]);
					if ~no_stage_2
						fprintf(out_hdl, ["  ev_",stages{2},"_t ", FPTR, "act_",stages{2},"_xmit[2], ", FPTR, "inact_",stages{2},"_xmit[2];\n"]);
					end
					fprintf(out_hdl, ["  se_",stages{1},"_t ", FPTR, "act_",stages{1},"_rcv[2], ", FPTR, "inact_",stages{1},"_rcv[2];\n"]);
					if ~no_stage_2
						fprintf(out_hdl, ["  se_",stages{2},"_t ", FPTR, "act_",stages{2},"_rcv[2], ", FPTR, "inact_",stages{2},"_rcv[2];\n"]);
					end
					fprintf(out_hdl, "#else\n");
					if rpage
						fprintf(out_hdl, "#pragma DATA_SEG __RPAGE_SEG PAGED_RAM\n");
					end
					fprintf(out_hdl, ["  se_",stages{1},"_t ",stages{1},"_xmit_buff_1_A"]);
					if ~isempty(se_defines{3,1})
						fprintf(out_hdl, " = {{\n");
						fprintf(out_hdl, "%s", se_defines{3,1}); %EV xmit ID strings
						fprintf(out_hdl, "  }};\n");
					else
						fprintf(out_hdl, ";\n");
					end
					fprintf(out_hdl, ["  se_",stages{1},"_t ",stages{1},"_xmit_buff_2_A"]);
					if ~isempty(se_defines{3,1})
						fprintf(out_hdl, " = {{\n");
						fprintf(out_hdl, "%s", se_defines{3,1}); %EV xmit ID strings
						fprintf(out_hdl, "  }};\n");
					else
						fprintf(out_hdl, ";\n");
					end
					fprintf(out_hdl, ["  se_",stages{1},"_t ",stages{1},"_xmit_buff_1_B"]);
					if ~isempty(se_defines{3,1})
						fprintf(out_hdl, " = {{\n");
						fprintf(out_hdl, "%s", se_defines{3,1}); %EV xmit ID strings
						fprintf(out_hdl, "  }};\n");
					else
						fprintf(out_hdl, ";\n");
					end
					fprintf(out_hdl, ["  se_",stages{1},"_t ",stages{1},"_xmit_buff_2_B"]);
					if ~isempty(se_defines{3,1})
						fprintf(out_hdl, " = {{\n");
						fprintf(out_hdl, "%s", se_defines{3,1}); %EV xmit ID strings
						fprintf(out_hdl, "  }};\n");
					else
						fprintf(out_hdl, ";\n");
					end
					if ~no_stage_2
						fprintf(out_hdl, ["  se_",stages{2},"_t ",stages{2},"_xmit_buff_1_A"]);
						if ~isempty(se_defines{3,2})
							fprintf(out_hdl, " = {{\n");
							fprintf(out_hdl, "%s", se_defines{3,2}); %SE xmit DATA strings
							fprintf(out_hdl, "  }};\n");
						else
							fprintf(out_hdl, ";\n");
						end
						fprintf(out_hdl, ["  se_",stages{2},"_t ",stages{2},"_xmit_buff_2_A"]);
						if ~isempty(se_defines{3,2})
							fprintf(out_hdl, " = {{\n");
							fprintf(out_hdl, "%s", se_defines{3,2}); %SE xmit DATA strings
							fprintf(out_hdl, "  }};\n");
						else
							fprintf(out_hdl, ";\n");
						end
						fprintf(out_hdl, ["  se_",stages{2},"_t ",stages{2},"_xmit_buff_1_B"]);
						if ~isempty(se_defines{3,2})
							fprintf(out_hdl, " = {{\n");
							fprintf(out_hdl, "%s", se_defines{3,2}); %SE xmit DATA strings
							fprintf(out_hdl, "  }};\n");
						else
							fprintf(out_hdl, ";\n");
						end
						fprintf(out_hdl, ["  se_",stages{2},"_t ",stages{2},"_xmit_buff_2_B"]);
						if ~isempty(se_defines{3,2})
							fprintf(out_hdl, " = {{\n");
							fprintf(out_hdl, "%s", se_defines{3,2}); %SE xmit DATA strings
							fprintf(out_hdl, "  }};\n");
						else
							fprintf(out_hdl, ";\n");
						end
					end
					if ~no_stage_2
						fprintf(out_hdl, ["  ev_",stages{2},"_t ",stages{2},"_rcv_buff_1_A, ",stages{2},"_rcv_buff_2_A, ",stages{2},"_rcv_buff_1_B, ",stages{2},"_rcv_buff_2_B;\n"]);
					end
					fprintf(out_hdl, ["  ev_",stages{1},"_t ",stages{1},"_rcv_buff_1_A, ",stages{1},"_rcv_buff_2_A, ",stages{1},"_rcv_buff_1_B, ",stages{1},"_rcv_buff_2_B;\n"]);
					if rpage
						fprintf(out_hdl, "#pragma DATA_SEG DEFAULT\n\n");
					end
					fprintf(out_hdl, ["  se_",stages{1},"_t ", FPTR, "act_",stages{1},"_xmit[2], ", FPTR, "inact_",stages{1},"_xmit[2];\n"]);
					if ~no_stage_2
						fprintf(out_hdl, ["  se_",stages{2},"_t ", FPTR, "act_",stages{2},"_xmit[2], ", FPTR, "inact_",stages{2},"_xmit[2];\n"]);
					end
					fprintf(out_hdl, ["  ev_",stages{1},"_t ", FPTR, "act_",stages{1},"_rcv[2], ", FPTR, "inact_",stages{1},"_rcv[2];\n"]);
					if ~no_stage_2
						fprintf(out_hdl, ["  ev_",stages{2},"_t ", FPTR, "act_",stages{2},"_rcv[2], ", FPTR, "inact_",stages{2},"_rcv[2];\n"]);
					end
					fprintf(out_hdl, "#endif\n\n");
				else
					fprintf(out_hdl, "%s", ev_defines{1,1}); %EV ID
					fprintf(out_hdl, "%s", se_defines{1,1}); %SE ID
					if ~no_stage_2
						fprintf(out_hdl, "%s", ev_defines{1,2}); %EV DATA
						fprintf(out_hdl, "%s", se_defines{1,2}); %SE DATA
					end
					fprintf(out_hdl, "\n");
					fprintf(out_hdl, "%s", se_defines{2,1}); %EV rcv ID strings
					if ~no_stage_2
						fprintf(out_hdl, "%s", se_defines{2,2}); %EV rcv DATA strings
					end
					fprintf(out_hdl, "%s", ev_defines{2,1});%SE rcv ID strings
					if ~no_stage_2
						fprintf(out_hdl, "%s", ev_defines{2,2});%SE rcv DATA strings
					end
				end

				state = 'delete';
			case 'pgs_to_send'
				fprintf(out_hdl, "#ifdef EV_CONFIG\n");
				gen_xmit_switch(out_hdl, 'ev', 0, ev_defined_pages, ev_array, true, FPTR, slash);
				if ~no_stage_2;
					gen_xmit_switch(out_hdl, 'ev', 1, ev_defined_pages, ev_array, true, FPTR, slash);
				end
				fprintf(out_hdl, "#else\n");
				gen_xmit_switch(out_hdl, 'se', 0, se_defined_pages, se_array, true, FPTR, slash);
				if ~no_stage_2;
					gen_xmit_switch(out_hdl, 'se', 1, se_defined_pages, se_array, true, FPTR, slash);
				end
				fprintf(out_hdl, "#endif\n");

				state = 'delete';
			case {'ids_init()','j3072_init()'}
				fprintf(out_hdl, "\n");
				fprintf(out_hdl, "#ifdef EV_CONFIG\n");
				fprintf(out_hdl, ["void clear_",stages{1},"_xmit_buff(ev_",stages{1},"_t ", FPTR, stages{1},"_xmit_buff)  {\n"]);
				if ~isempty(ev_defines{1,1})
					fprintf(out_hdl, ["  mem_init(&(",stages{1},"_xmit_buff->v), (uint8_t)sizeof(ev_",stages{1},"_vars_t), 0xFF);\n"]);
				end
				fprintf(out_hdl, "}\n");
				fprintf(out_hdl, ["void clear_",stages{1},"_rcv_buff(se_",stages{1},"_t ", FPTR, stages{1},"_rcv_buff)  {\n"]);
				if ~isempty(se_defines{1,1})
					fprintf(out_hdl, ["  mem_init(&(",stages{1},"_rcv_buff->v), (uint8_t)sizeof(se_",stages{1},"_vars_t), 0xFF);\n"]);
				end
				if ~isempty(se_defines{2,1})
					fprintf(out_hdl, ["  mem_init(&(",stages{1},"_rcv_buff->s), (uint8_t)sizeof(se_",stages{1},"_strings_t), 0x00);\n"]);
				end
				fprintf(out_hdl, "}\n");
				if ~no_stage_2
					fprintf(out_hdl, ["void clear_",stages{2},"_xmit_buff(ev_",stages{2},"_t ", FPTR,stages{2},"_xmit_buff)  {\n"]);
					if ~isempty(ev_defines{1,2})
						fprintf(out_hdl, ["  mem_init(&(",stages{2},"_xmit_buff->v), (uint8_t)sizeof(ev_",stages{2},"_vars_t), 0xFF);\n"]);
					end
					if ~isempty(ev_defines{2,2})
						fprintf(out_hdl, ["  mem_init(&(",stages{2},"_xmit_buff->s), (uint8_t)sizeof(ev_",stages{2},"_strings_t), 0x00);\n"]);
					end
					fprintf(out_hdl, "}\n");
					fprintf(out_hdl, ["void clear_",stages{2},"_rcv_buff(se_",stages{2},"_t ", FPTR, stages{2},"_rcv_buff)  {\n"]);
					if ~isempty(se_defines{1,2})
						fprintf(out_hdl, ["  mem_init(&(",stages{2},"_rcv_buff->v), (uint8_t)sizeof(se_",stages{2},"_vars_t), 0xFF);\n"]);
					end
					if ~isempty(se_defines{2,2})
						fprintf(out_hdl, ["  mem_init(&(",stages{2},"_rcv_buff->s), (uint8_t)sizeof(se_",stages{2},"_strings_t), 0x00);\n"]);
					end
					fprintf(out_hdl, "}\n");
				end
				fprintf(out_hdl, "#else\n");
				fprintf(out_hdl, ["void clear_",stages{1},"_xmit_buff(se_",stages{1},"_t ", FPTR, stages{1},"_xmit_buff)  {\n"]);
				if ~isempty(ev_defines{1,1})
					fprintf(out_hdl, ["  mem_init(&(",stages{1},"_xmit_buff->v), (uint8_t)sizeof(se_",stages{1},"_vars_t), 0xFF);\n"]);
				end
				fprintf(out_hdl, "}\n");
				fprintf(out_hdl, ["void clear_",stages{1},"_rcv_buff(ev_",stages{1},"_t ", FPTR, stages{1},"_rcv_buff)  {\n"]);
				if ~isempty(se_defines{1,1})
					fprintf(out_hdl, ["  mem_init(&(",stages{1},"_rcv_buff->v), (uint8_t)sizeof(ev_",stages{1},"_vars_t), 0xFF);\n"]);
				end
				if ~isempty(se_defines{2,1})
					fprintf(out_hdl, ["  mem_init(&(",stages{1},"_rcv_buff->s), (uint8_t)sizeof(ev_",stages{1},"_strings_t), 0x00);\n"]);
				end
				fprintf(out_hdl, "}\n");
				if ~no_stage_2
					fprintf(out_hdl, ["void clear_",stages{2},"_xmit_buff(se_",stages{2},"_t ", FPTR, stages{2},"_xmit_buff)  {\n"]);
					if ~isempty(se_defines{1,2})
						fprintf(out_hdl, ["  mem_init(&(",stages{2},"_xmit_buff->v), (uint8_t)sizeof(se_",stages{2},"_vars_t), 0xFF);\n"]);
					end
					if ~isempty(se_defines{2,2})
						fprintf(out_hdl, ["  mem_init(&(",stages{2},"_xmit_buff->s), (uint8_t)sizeof(se_",stages{2},"_strings_t), 0x00);\n"]);
					end
					fprintf(out_hdl, "}\n");
					fprintf(out_hdl, ["void clear_",stages{2},"_rcv_buff(ev_",stages{2},"_t ", FPTR, stages{2},"_rcv_buff)  {\n"]);
					if ~isempty(ev_defines{1,2})
						fprintf(out_hdl, ["  mem_init(&(",stages{2},"_rcv_buff->v), (uint8_t)sizeof(ev_",stages{2},"_vars_t), 0xFF);\n"]);
					end
					if ~isempty(ev_defines{2,2})
						fprintf(out_hdl, ["  mem_init(&(",stages{2},"_rcv_buff->s), (uint8_t)sizeof(ev_",stages{2},"_strings_t), 0x00);\n"]);
					end
					fprintf(out_hdl, "}\n");
				end
				fprintf(out_hdl, "#endif\n\n");
				fprintf(out_hdl, ["\nvoid ",{"ids","j3072"}{slash},"_init(uint8_t ch) {\n"]);
				switch slash
					case 1
						fprintf(out_hdl, "  se_id_status[ch] = ID_INCOMPLETE;\n");
						fprintf(out_hdl, "  ev_id_status[ch] = ID_INCOMPLETE;\n");
					case 2
						fprintf(out_hdl, "  se_j3072_status[ch] = CERT_INCOMPLETE;\n");
						fprintf(out_hdl, "  ev_j3072_status[ch] = CERT_INCOMPLETE;\n");
				end
				fprintf(out_hdl, "  first_data[ch]= false;\n\n");
				fprintf(out_hdl, ["  act_",stages{1},"_xmit[ch] = ch ? &",stages{1},"_xmit_buff_1_B : &",stages{1},"_xmit_buff_1_A;\n"]);
				fprintf(out_hdl, ["  inact_",stages{1},"_xmit[ch] = ch ? &",stages{1},"_xmit_buff_2_B : &",stages{1},"_xmit_buff_2_A;\n"]);
				fprintf(out_hdl, ["  act_",stages{1},"_rcv[ch] = ch ? &",stages{1},"_rcv_buff_1_B : &",stages{1},"_rcv_buff_1_A;\n"]);
				fprintf(out_hdl, ["  inact_",stages{1},"_rcv[ch] = ch ? &",stages{1},"_rcv_buff_2_B : &",stages{1},"_rcv_buff_2_A;\n\n"]);
				if ~no_stage_2
					fprintf(out_hdl, ["  act_",stages{2},"_xmit[ch] = ch ? &",stages{2},"_xmit_buff_1_B : &",stages{2},"_xmit_buff_1_A;\n"]);
					fprintf(out_hdl, ["  inact_",stages{2},"_xmit[ch] = ch ? &",stages{2},"_xmit_buff_2_B : &",stages{2},"_xmit_buff_2_A;\n"]);
					fprintf(out_hdl, ["  act_",stages{2},"_rcv[ch] = ch ? &",stages{2},"_rcv_buff_1_B : &",stages{2},"_rcv_buff_1_A;\n"]);
					fprintf(out_hdl, ["  inact_",stages{2},"_rcv[ch] = ch ? &",stages{2},"_rcv_buff_2_B : &",stages{2},"_rcv_buff_2_A;\n\n"]);
				end

				fprintf(out_hdl, ["  clear_",stages{1},"_xmit_buff(act_",stages{1},"_xmit[ch]);\n"]);
				fprintf(out_hdl, ["  clear_",stages{1},"_xmit_buff(inact_",stages{1},"_xmit[ch]);\n"]);
				fprintf(out_hdl, ["  clear_",stages{1},"_rcv_buff(act_",stages{1},"_rcv[ch]);\n"]);
				fprintf(out_hdl, ["  clear_",stages{1},"_rcv_buff(inact_",stages{1},"_rcv[ch]);\n"]);
				if ~no_stage_2
					fprintf(out_hdl, ["  clear_",stages{2},"_xmit_buff(act_",stages{2},"_xmit[ch]);\n"]);
					fprintf(out_hdl, ["  clear_",stages{2},"_xmit_buff(inact_",stages{2},"_xmit[ch]);\n"]);
					fprintf(out_hdl, ["  clear_",stages{2},"_rcv_buff(act_",stages{2},"_rcv[ch]);\n"]);
					fprintf(out_hdl, ["  clear_",stages{2},"_rcv_buff(inact_",stages{2},"_rcv[ch]);\n\n"]);
				end

				fprintf(out_hdl, ["  ",{"id","cert"}{slash},"_xmit(ch, 0); //preload the control page\n\n"]);
				if gen_values
					fprintf(out_hdl, "%s\n\n", '  srand((unsigned) time(NULL));   //seed random number generator');
				end
				fprintf(out_hdl, "#ifdef EV_CONFIG\n");

				if gen_values
					fprintf(out_hdl, ["  #define PREPEND (act_",stages{1},"_xmit[ch]->v).\n"]);
					fprintf(out_hdl, "%s", ev_inits{3,1}); %EV xmit ID
					fprintf(out_hdl, "  #undef PREPEND\n");
					fprintf(out_hdl, ["  #define PREPEND (act_",stages{2},"_xmit[ch]->v).\n"]);
					fprintf(out_hdl, "%s", ev_inits{3,2}); %EV xmit DATA
					fprintf(out_hdl, "  #undef PREPEND\n");
				end
				fprintf(out_hdl, "#else\n");
				if gen_values
					fprintf(out_hdl, ["  #define PREPEND (act_",stages{1},"_xmit[ch]->v).\n"]);
					fprintf(out_hdl, "%s", se_inits{3,1});%SE xmit ID
					fprintf(out_hdl, "  #undef PREPEND\n");
					fprintf(out_hdl, ["  #define PREPEND (act_",stages{2},"_xmit[ch]->v).\n"]);
					fprintf(out_hdl, "%s", se_inits{3,2});%SE xmit DATA
					fprintf(out_hdl, "  #undef PREPEND\n");
				end
				fprintf(out_hdl, "#endif\n");
				if gen_values
					fprintf(out_hdl, "%s\n", '  printf("======================================================================\n");');
				end
				fprintf(out_hdl, "}\n\n");

				state = 'delete';
			case {'ev_id_parse','ev_cert_parse'}
				if ~capl
					gen_parse_switch(out_hdl, 'se', 0, se_defined_pages, se_array, print_command, FPTR, false, slash);	%se because ev receives se data
				else
					gen_parse_switch(out_hdl, 'ev', 0, ev_defined_pages, ev_array, print_command, FPTR, true, slash);
				end

				state = 'delete';
			case {'ev_id_xmit','ev_cert_xmit'}
				gen_xmit_switch(out_hdl, 'ev', 0, ev_defined_pages, ev_array, false, FPTR, slash);

				state = 'delete';
			case {'ev_data_parse','ev_sunspec_parse'}
				if ~capl
					gen_parse_switch(out_hdl, 'se', 1, se_defined_pages, se_array, print_command, FPTR, false, slash);	%se because ev receives se data
				else
					gen_parse_switch(out_hdl, 'ev', 1, ev_defined_pages, ev_array, print_command, FPTR, true, slash);
				end

				state = 'delete';
			case {'ev_data_xmit','ev_sunspec_xmit'}
				gen_xmit_switch(out_hdl, 'ev', 1, ev_defined_pages, ev_array, false, FPTR, slash);

				state = 'delete';

			case {'se_id_parse','se_cert_parse'}
				if ~capl
					gen_parse_switch(out_hdl, 'ev', 0, ev_defined_pages, ev_array, print_command, FPTR, false, slash);	%ev because se receives ev data
				else
					gen_parse_switch(out_hdl, 'se', 0, se_defined_pages, se_array, print_command, FPTR, true, slash);
				end

				state = 'delete';
			case {'se_id_xmit','se_cert_xmit'}
				gen_xmit_switch(out_hdl, 'se', 0, se_defined_pages, se_array, false, FPTR, slash);

				state = 'delete';
			case {'se_data_parse','se_sunspec_parse'}
				if ~capl
					gen_parse_switch(out_hdl, 'ev', 1, ev_defined_pages, ev_array, print_command, FPTR, false, slash);
				else
					gen_parse_switch(out_hdl, 'se', 1, se_defined_pages, se_array, print_command, FPTR, true, slash);
				end

				state = 'delete';
			case {'se_data_xmit','se_sunspec_xmit'}
				gen_xmit_switch(out_hdl, 'se', 1, se_defined_pages, se_array, false, FPTR, slash);

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
