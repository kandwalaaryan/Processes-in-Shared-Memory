#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
pid_t pid;
	
	/* Fork a child process */
	pid = fork();

	if(pid<0){ /* Error occured */
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if(pid == 0){ /* Child Process */
		execlp("/bin/ls", "ls",NULL);
	}
	else { /* Parent Process */
		/* Parent will wait for the child to complete */
		sleep(10);
		wait(NULL);
		printf("Child Complete");
	}
	return 0;
}
