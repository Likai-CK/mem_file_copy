#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
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
	int source_fd, dest_fd, inode;
	int filesize = get_file_size(source_path);
	source_fd = open(source_path, O_RDONLY); // read only (we are copying this file.
	source_data = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, source_fd, 0);
	
	dest_fd = open(dest_path, O_RDWR | O_CREAT, 0666); // the last arg is mode_t permissions. See ls file
	ftruncate(dest_fd, filesize); 
	// https://linux.die.net/man/2/ftruncate - this gets rid of extra data after moving a file
	// https://stackoverflow.com/questions/26582920/mmap-memcpy-to-copy-file-from-a-to-b
	// If the filesize specified is not correct, the destination filesize will differ. An error in specifying the correct filesize was made in the SO page.
	dest = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dest_fd, 0);
	memcpy(dest, 
}
int main(int argc, char **argv){
	
	mem_copy_file("cat.txt", "cat2.txt");
	return 0
}