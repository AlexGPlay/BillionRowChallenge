build-line-by-line: line_by_line.c 
	gcc -O3 line_by_line.c -o line_by_line.out
time-line-by-line: build-line-by-line
	time ./line_by_line.out