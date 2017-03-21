#include <stdio.h>
#include <stdlib.h>
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
	/* pointer to shared memory obect */
	struct filesharing_struct *ptr;

	/* open the shared memory object */
	shm_fd = shm_open(name, O_RDONLY, 0666);

	/* memory map the shared memory object */
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

	//If the flag is 1 then something was uploaded
	if(ptr->flag == 1){
		//Sets the filename for the file written to.
		char *filename = "./download.txt";
		int fd = open(filename,O_CREAT|O_WRONLY,0666);
		//Writes to the file, from the structs data
		if(write(fd, ptr->data,ptr->size) != ptr->size){
			fprintf(stderr,"Error writing to file %s",filename);
		}
	} else {
		printf("No file to be downloaded \n");
		fflush(stdout);
	}
	
	/* remove the shared memory object */
	shm_unlink(name);

	return 0;
}
