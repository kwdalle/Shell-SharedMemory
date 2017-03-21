The fork() call is used to create a new process. That new process has a copy of the 
original processe's adress space. This is done so that bothe the child process and
the parent are able to communicate. Parent and child resume execution after this
call.

execlp() is a version of the sytem call exec(). This system call is used replace
the processes memory space with a different program. It loads a binary file into
memory, destroying the image of the child process, and then starts the execution
of the new binary file. This allows for the processes to communicate and then go
their separate ways.

wait() is used to remove itself from the ready queue until the child process is 
done executing. exec() does not return control to the parent process that does
this unless there was an error.
