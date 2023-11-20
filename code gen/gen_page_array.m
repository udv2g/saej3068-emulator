function [defined_pages, array] = gen_page_array(rawarr, ident)

array = [];

defined_pages = [];

	max_rows = size(rawarr)(1);
	tot_rows = 0;

	row = 2;
	while row <= max_rows && ~isempty(rawarr(row,2){1})
		if ~strcmp(rawarr(row,1){1}, '!!')	%ignore if '!!' in Revised column
			tot_rows = row;
			page = rawarr(row,2){1};
			if ~ismember(page,defined_pages)
				defined_pages = [defined_pages, page];
			end
		end
		row++;
	end

	for page = defined_pages
		index = 1;
		for row = [2:tot_rows]
			if rawarr(row,2){1} == page && ~strcmp(rawarr(row,1){1}, '!!')
				[struct.page_number, struct.stage, struct.multipage, struct.start_position, struct.end_position, struct.bit_start, struct.bytes_bits, struct.label, struct.scale, struct.offset, struct.unit] = parse_row(rawarr, row, ident);
				array{page}(index) = struct;
				index++;
			end
		end
	end
end