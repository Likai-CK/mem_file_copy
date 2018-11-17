#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <limits.h>

/*

Write a program, analagous to cp1), that uses mmap() and memcpy() calls (instead of read() or write()) to copy a source file
to a destination file. (Use fstat() to obtain the size of the input file, which can then be used to size the required memory
mappings, and use ftruncate() to set the size of the output file.

https://linux.die.net/man/2/ftruncate
http://man7.org/linux/man-pages/man2/open.2.html
http://man7.org/linux/man-pages/man1/cp.1.html
https://stackoverflow.com/questions/26582920/mmap-memcpy-to-copy-file-from-a-to-b

*/

int get_file_size(char *path){
	struct stat st;
	stat(path, &st);
	int size = st.st_size;
	return size;
}

int mem_copy_file(char *source_path, char *dest_path){
	int source_fd, dest_fd;
	char *source_data, *dest_data;
	int filesize = get_file_size(source_path);
	source_fd = open(source_path, O_RDONLY); // read only (we are copying this file.
	source_data = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, source_fd, 0); // the last number is where to START reading in the file - you can start at some other point in the file, too.
	// ^ this loads the file INTO MEMORY.
	
	dest_fd = open(dest_path, O_RDWR | O_CREAT, 0666); // the last arg is mode_t permissions. See ls file
	dest_data = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dest_fd, 0); // loads the dest file into memory.
	// Making this shared is important, otherwise the changes we make won't be written back to the file.
	
	ftruncate(dest_fd, filesize); 
	// https://linux.die.net/man/2/ftruncate - this gets rid of extra data after moving a file
	// https://stackoverflow.com/questions/26582920/mmap-memcpy-to-copy-file-from-a-to-b
	// If the filesize specified is not correct, the destination filesize will differ. An error in specifying the correct filesize was made in the SO page.
	
	memcpy(dest_data, source_data, filesize);
	
	munmap(source_data, filesize); // unmap the data (memory leak concern?)
	munmap(dest_data, filesize);
	close(dest_fd);
	close(source_fd); // close the file descriptors.
	
	return 1;
}
// note: argv[0] is the program name.
// USAGE: file_copy input_filename destination_filename
// Copies input to destination filename using memcpy and no read/write functions.
int main(int argc, char **argv){
	if(argc < 3){
		printf("Not enough args. Usage: file_copy src_filename dest_filename");	
	}
	else if(argc > 3){
		printf("Too many args. Usage: file_copy src_filename dest_filename");
	}
	else{
		printf("Copying %s to %s.\n", argv[1], argv[2]);
		mem_copy_file(argv[1], argv[2]);
	}
	mem_copy_file("cat.txt", "cat2.txt");
	return 0;
}