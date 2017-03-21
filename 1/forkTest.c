#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main (){
	pid_t pid;

	/* fork a child process */
	pid = fork();

	if (pid < 0){ /* error occured */
		fprintf(stderr, "Fork Failed");
		return 1;
	} else if (pid == 0){ /* Child Process */
		execlp("/bin/ls","ls",NULL);
	} else { /* Parent Process */
		wait(NULL);
		printf("Child Complete\n");
	}

	return 0;
}
