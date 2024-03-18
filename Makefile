build-line-by-line: line_by_line.c 
	gcc -O3 line_by_line.c -o line_by_line.out
time-line-by-line: build-line-by-line
	time ./line_by_line.out
build-buffering: buffering.c 
	gcc -O3 buffering.c -o buffering.out
time-buffering: build-buffering
	time ./buffering.out
build-mmap: mmap.c 
	gcc -O3 mmap.c -o mmap.out
time-mmap: build-mmap
	time ./mmap.out