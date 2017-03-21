parent: pid = 9732
parent: pid1 = 9731
child: pid = 0
child: pid1 = 9732

getpid() in the context of the child process will return the process id of the parent process. In general it returns the process id of the calling process. It is used for processes that generate unique temporary filenames as well.

fprintf() prints things in a stream. If a file is not given it is printed to the console. Every paramter after the string to be printed is substituted into embedded format tags within the string.
