shm_open() creates the shared memory object that is associated with a region of shared memory with a file. The first parameter in this system call is the name of the shared memory object. Next, you must specify that if the memory object does not exist than create it (O_CREAT). You must also specify what kind
of operations can be done on that file (Read/Write). The final parameter is what gives the directory 
permissions for the object (because it is treated like a file). If this call is successful it will
return an integer that is a file descriptor for this memory object.

ftruncate() is used to configure the size of the object. The only parameters are, in order, the file descriptor and the amount of bytes you want the object's size to be. 

mmap() is the main function in creating a shared memory object. This function's purpose is to create a file containing the memory-mapped object. It will return a pointer to the memory-mapped object. This
pointer will then be used to access the memory-mapped file object in the future. 

shm_unlink() is used to get rid of the memory-mapped object. The only parameter for this system call is the name of the memory-mapped object. This is done by the consumer in the relationship (obviously after the consumer has accessed it). 
