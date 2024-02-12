#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

// Define the BUFFER_SIZE to 25, READ_END to 0 and WRITE_END to 1 to use later
#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

// Global variables that will be used to store values of the child process end, 
// the parent process end and the time elapsed in between
struct timeval child_current;
struct timeval parent_current;
struct timeval time_elapsed;

int main(int argc, char *argv[]){
        // Declares and initializes the pid, and initializes the file descriptor
	pid_t pid;
	// File descriptor has a size of 2 (0 for read and 1 for write, as defined earlier)
	int fd[2];
        
        // If creating the pipe returns -1, then the pipe has failed
	if(pipe(fd) == -1){
		fprintf(stderr, "Pipe failed");
		return 1;
	}
        // Start forking process
	pid = fork();
        
        // If the pid is less than 0, then the fork has failed, so terminate the program
	if(pid < 0){
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if(pid > 0){  // parent process 
	        // wait for child process to run command
		wait(NULL);
		
		// Read the value of the child-current from the pipe
		read(fd[READ_END], &child_current, sizeof(struct timeval));
      
                // Gets the current time and stores it in parent_current
		gettimeofday(&parent_current, NULL);
		
                // subtract child's current time from parent's, then print
		timersub(&parent_current, &child_current, &time_elapsed);
		printf("Elapsed time: %ld.%ld\n", time_elapsed.tv_sec, time_elapsed.tv_usec);
                
                // Close the read end
		close(fd[READ_END]);
	}
	else{   // child process
	        // Close read so we can write
		close(fd[READ_END]);
		// Gets the current time and stores it in child_current
		gettimeofday(&child_current, NULL);
		// Write the value of child_current into the in the pipe for the parent to later read
		write(fd[WRITE_END], &child_current, sizeof(struct timeval));
		// Close write so we can read
		close(fd[WRITE_END]);
                
                // executes command
		execvp(argv[1], &argv[1]);
	}
	return 0;
}

