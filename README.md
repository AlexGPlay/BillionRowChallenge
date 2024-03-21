So 1 billion rows challenge or [1BRC](https://github.com/ifnesi/1brc)...

I implemented it in three variants, each of them single and multithreaded.

The singlethreaded version of each of them is just sequentially reading the file in a different approach and the multithreaded is the same thing but chunking the file for each of the threads.

Let me explain each of them even if they sounds pretty straightforward:

- Line by line: Just read each line independently using fgets, which by itself reads until there's an newline character (\n) or the end of file is reached.
- Buffering: Instead of line by line it reads a whole chunk of bytes from the file, this should reduce the number of read syscalls. For this fread is used.
- Mmap: A syscall that maps the whole file to a section of memory. This should be the fastest way of reading a file since it minimizes the number of syscalls and just lets the OS load the file as it prefers.

### Benchmarking

All the tests have been run on a Apple Macbook Pro (M1 Pro) with 32GB of RAM.

| Approach     | Single/Multi thread | Time (seconds) |
| ------------ | ------------------- | -------------- |
| Line by line | Singlethread        | 109.63s        |
| Line by line | Multithread         | 269.36s        |
| Buffering    | Singlethread        | 104.53s        |
| Buffering    | Multithread         | 26.96s         |
| Mmap         | Singlethread        | 115.10s        |
| Mmap         | Multithread         | 20.32s         |

### How to use

The Makefile of this project has instructions to compile and run both the single and multi threaded versions, they are the following ones:

- build-line-by-line
- time-line-by-line
- build-line-by-line-multi
- time-line-by-line-multi
- build-buffering
- time-buffering
- build-buffering-multi
- time-buffering-multi
- build-mmap
- time-mmap
- build-mmap-multi
- time-mmap-multi

There's no need to build before using the time command as it requires the build step. Simply calling the time one will compile.

So just `make time-(line-by-line/buffering/mmap)[-multi]`
