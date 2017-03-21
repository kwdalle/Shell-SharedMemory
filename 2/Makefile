all: shm shm2

shm: shm.c
	gcc -lrt -o shm shm.c

shm2: shm2.c
	gcc -lrt -o shm2 shm2.c

clean:
	$(RM) shm shm2
