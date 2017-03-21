#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include "filesharing_struct.h"
int main()
{
	/* the size (in bytes) of shared memory object */
	const int SIZE = 10000000;
	/* name of the shared memory object */
	const char *name = "kwdalle_filesharing";


	/* shared memory file descriptor */
	int shm_fd;

	/* create the shared memory object */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory object */
	ftruncate(shm_fd, SIZE);
	
	//Max filename length
	const int MAX_FILE = 80;
	char input[MAX_FILE];
	
	fprintf(stdout, "Please enter the file name: ");
	fflush(stdout);

	fgets(input, MAX_FILE, stdin);

	if(input[strlen(input)-1] == '\n')
		input[strlen(input)-1] = '\0';

	int fd = open(input, O_RDONLY, 0666);

	//Creates the stat struct to get necessary information
	struct stat s;
	stat(input, &s);

	struct filesharing_struct *fs;
	/* memory map the shared memory object */
	fs = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	fs->size = s.st_size;
	//Reads the file into the structs array
	if(read(fd,fs->data,fs->size) != fs->size){
		//If it failed set it the flag to zero
		fs->flag = 0;
		printf("Error reading from file\n");
		return 1;
	} else {
		//If it read in correctly set the flag to 1
		fs->flag = 1;
	}
	close(fd);
	return 0;
}
