pipe() is the system call that creates the pipe. It takes a single argument that is an array of length 2. Inside this array are two file descriptors that correspond to the read and write ends of the pipe. Pipes are unidirectional, thus if both processes are suppose to be able to communicate than at least two pipes are needed for this functionality. 

write() is the system call that is used to send messages through the pipe. This is because UNIX treats pipes as a special kind of of file, thus allowing for them to use the API for files.

close() can be used to close the different "ends" of the pipe via the different file descriptors. This is done after you are done using a particular end, or it is used to close an end of the pipe that will never be used by that process. In this code the if block that wrote to the pipe closed the read portion, and the if that did the reading closed the write portion.
