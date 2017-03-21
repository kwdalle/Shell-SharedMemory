all: shm_fileuploader shm_filedownloader

shm_fileuploader: shm_fileuploader.c filesharing_struct.h
	gcc -lrt -o shm_fileuploader shm_fileuploader.c

shm_filedownloader: shm_filedownloader.c filesharing_struct.h
	gcc -lrt -o shm_filedownloader shm_filedownloader.c

clean:
	$(RM) shm_fileuploader shm_filedownloader download.txt
