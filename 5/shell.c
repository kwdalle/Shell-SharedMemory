#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LINE 80 /* maximum length command */
#define MAX_HISTORY 100

int main(void){
	char *args[MAX_LINE/2 +1]; /* command line arguments */
	int should_run = 1;
	int background = 0;
	int PIPE = 0;
	int redirect = 0;
	int his = 0;
	int hcmd = 0;
	int repeat = 0;
	int repeat1 = -1;
	char history [MAX_HISTORY][MAX_HISTORY];
	
	while (should_run){
		printf("dalle_osh>");
		fflush(stdout);
		char input[MAX_LINE];

		fgets(input, MAX_LINE, stdin);

		//Looks for the start of a history command
		if (input[0] == '!'){
			char number[15];
			int k = 1;
			while (isdigit(input[k])){
				number[k-1] = input[k];
				k += 1;
			} 
			repeat1 = atoi(number);
		} else{
			strcpy(history[his], input);
			his += 1;
		}
		
		//The token that will be used to split the input string.
		char *token;
		
		token = strtok(input, " \n");
	
		int i = 0;
		//Goes through the tokens and looks for certain things for flags.
		while (token != NULL){
			if (strcmp(token,"&") == 0){
				background = 1;
			} else if (strcmp(token, "history") == 0){
				hcmd = 1;
			} else if (strcmp(token, "!!") == 0){
				repeat = 1;
			}else if (strcmp(token, "|") == 0){
				PIPE = i;
				args[i] = token;
				i+=1;
			}else if (strcmp(token, ">") == 0){
				redirect = i;
				args[i] = token;
				i += 1;
			}else if (strcmp(token, "exit") == 0){
				should_run = 0;
			}else{
				args[i] = token;
				i+=1;
			}
			token = strtok(NULL, " \n");
		}
		args[i] = NULL;

		pid_t pid;

		//For history commands depending on the type.
		if (repeat > 0){
			token = strtok(history[his-1], " \n");
			int k = 0;

			while (token != NULL){
				if (strcmp(token,"&") == 0){
					background = 1;
				} else if (strcmp(token, "history") == 0){
					hcmd = 1;
				}else if (strcmp(token, "|") == 0){
					PIPE = k;
					args[k] = token;
					k+=1;
				}else if (strcmp(token, ">") == 0){
					redirect = k;
					args[k] = token;
					k += 1;
				}else{
					args[k] = token;
					k+=1;
				}
				token = strtok(NULL, " \n");
			}
			args[k] = NULL;
			repeat = 0;
		} else if (repeat1 > -1){
			token = strtok(history[repeat1], " \n");
			int k = 0;

			while (token != NULL){
				if (strcmp(token,"&") == 0){
					background = 1;
				} else if (strcmp(token, "history") == 0){
					hcmd = 1;
				}else if (strcmp(token, "|") == 0){
					PIPE = k;
					args[k] = token;
					k+=1;
				}else if (strcmp(token, ">") == 0){
					redirect = k;
					args[k] = token;
					k += 1;
				}else{
					args[k] = token;
					k+=1;
				}
				token = strtok(NULL, " \n");
			}
			args[k] = NULL;
			repeat1 = -1;
		}
		
		//Prints the history out or redirects or does a pipe command and if none of those are there does a normal command.
		if (hcmd > 0){
			int k = 0;
			while (k < his){
				printf("%d: %s\n", k, history[k]);
				k += 1;
			}
			hcmd = 0;
		}else if (redirect > 0){
			//Forks and then executes after changing stdout to file.
			pid_t pid2;
			pid2 = fork();
			int saved_output = dup(1);
			int saved_input = dup(0);
			int outFile;
			if(pid2 ==0){
				//Splits up command and filename.
				char *first[MAX_LINE/2 +1];
				char *second[MAX_LINE/2 +1];
				int k = 0;
				while (k < redirect){
					first[k] = args[k];
					k+=1;
				}
				first[k] = NULL;	
				k += 1;
				outFile = open(args[k], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
				dup2(outFile,1);
				execvp(first[0], first);
			}else wait(NULL);
			close(outFile);
			//Resets output and resets the redirect flag.
			dup2(saved_input,0);
			redirect = 0;
		}else if (PIPE > 0){
			// Forks once and then in the child forks again to have one
			// process do the first command and another do the second.
			pid_t pid2;
			pid2 = fork();
			int saved_output = dup(1);
			int saved_input = dup(0);
			int pipes[2];
			pipe(pipes);
			if(pid2 ==0){
				char *first[MAX_LINE/2 +1];
				char *second[MAX_LINE/2 +1];
				int k = 0;
				while (k < PIPE){
					first[k] = args[k];
					k+=1;
				}
				first[k] = NULL;	
				k += 1;
				int l = 0;
				while (k < i){
					second[l] = args[k];
					l += 1;
					k+=1;
				}
				second[l] = NULL;
				pid_t childPid;
				childPid = fork();
				if(childPid < 0){
					fprintf(stderr, "Fork Failed");
					return 1;
				} else if (childPid == 0){
					close(pipes[0]);
					dup2(pipes[1],1);
					execvp(first[0],first);
				}else{	
					close(pipes[1]);
					dup2(pipes[0],0);
					execvp(second[0],second);
				}
			}else wait(NULL);
			//Closes the pipes and resets input.
			close(pipes[0]);
			close(pipes[1]);
			PIPE=0;
			dup2(saved_input,0);
		}else{
			// Forks and then has the child execute the command
			// If its not a background then the parent waits, 
			// otherwise the parent prints out a message and ends
			pid = fork();
			if (pid < 0){
				fprintf(stderr, "Fork Failed");
				return 1;
			} else if (pid == 0){
				execvp(args[0], args);
			}else{
				if (!background){	
					wait(NULL);
				} else {
					printf("[1]+ running (Pid: %d) ", pid);
					int k = 0;
					while(k < i){
						printf("%s ", args[k]);
						k+=1;
					}
					printf("&\n");
					fflush(stdout);
					background = 0;
				}
			}
		}
		i += 1;
	}
}

