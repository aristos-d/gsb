function dumpbinsparse(A, dumper, islogical)
% Neumann is little-endian
% M = UFGet(....)
% usage: dumpbinsparse(M.A, 'matrixname.bin', false)

f = fopen([dumper], 'w', 'l');

[i, j, v] = find(A); 

fwrite(f, size(A,1), 'uint32');
fwrite(f, size(A,2), 'uint32');
fwrite(f, nnz(A), 'uint32');

fwrite(f, (i-1), 'uint32');
fwrite(f, (j-1), 'uint32');

if(~ islogical)
	fwrite(f, v, 'double');
end

fclose(f);
