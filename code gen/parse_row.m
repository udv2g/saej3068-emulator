function [page_number, stage, multipage, start_position, end_position, bit_start, bytes_bits, label, scale, offset, unit] = parse_row(rawarr, row, ident)

	page_number = rawarr(row,2){1};

	switch rawarr(row,3){1}
		case 'ID'
			stage = 0;
		case 'DATA'
			stage = 1;
		case 'CRC32'
			stage = -2;
		otherwise
			stage = -1;
	end

	if strcmp(rawarr(row,4){1}, 'No')
		multipage = [];
	else
		multipage = [sscanf(rawarr(row,4){1},'%d'), sscanf(rawarr(row,4){1}(3:end),'%d')];
	end

	start_position = rawarr(row,5){1}(1) - 'A';
	bit_start = [];
	if size(rawarr(row,5){1})(2) == 1
		end_position = start_position;
	else
		if rawarr(row,5){1}(2) == '-'
			end_position = rawarr(row,5){1}(3) - 'A';
		else
			end_position = start_position;
			bit_start = str2double(rawarr(row,5){1}(3));
		end
	end

	bytes_bits = sscanf(rawarr(row,8){1},'%d');
	if isempty(bytes_bits)
		bytes_bits = sscanf(rawarr(row,8){1}(7:end),'%d');	%"up to ..."
	end

	label = strrep(rawarr(row,6){1},' ','_');

	if ~strncmp(ident, label, 2)
		label = [ident, label];
	end

	if isempty(rawarr(row,9){1})
		scale = [];
	else
		scale = sscanf(rawarr(row,9){1},'%f');
	end

	if isempty(rawarr(row,10){1})
		offset = [];
	else
		if isa(rawarr(row,10){1},'double')
			offset = rawarr(row,10){1};
		else
			offset = sscanf(rawarr(row,10){1},'%f');
		end
	end

	unit = rawarr(row,12){1};

end