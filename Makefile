build-generator: create_measurements.c
	gcc -O3 create_measurements.c -o create_measurements.out
generate-measurements: build-generator
	./create_measurements.out
build-line-by-line: line_by_line.c 
	gcc -O3 line_by_line.c -o line_by_line.out
time-line-by-line: build-line-by-line
	time ./line_by_line.out
build-line-by-line-multi: line_by_line_multi.c 
	gcc -O3 line_by_line_multi.c -o line_by_line_multi.out
time-line-by-line-multi: build-line-by-line-multi
	time ./line_by_line_multi.out
build-buffering: buffering.c 
	gcc -O3 buffering.c -o buffering.out
time-buffering: build-buffering
	time ./buffering.out
build-buffering-multi: buffering_multi.c 
	gcc -O3 buffering_multi.c -o buffering_multi.out
time-buffering-multi: build-buffering-multi
	time ./buffering_multi.out
build-mmap: mmap.c 
	gcc -O3 mmap.c -o mmap.out
time-mmap: build-mmap
	time ./mmap.out
build-mmap-multi: mmap_multi.c 
	gcc -O3 mmap_multi.c -o mmap_multi.out
time-mmap-multi: build-mmap-multi
	time ./mmap_multi.out