#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

// Global variables that will be used to store values of the child process end, 
// the parent process end and the time elapsed in between
struct timeval child_current;
struct timeval parent_current;
struct timeval time_elapsed;

int main(int argc, char *argv[])
{
        // Declares and initializes the pid, size of the shared memory and the name of the shared memory space
	pid_t pid;
	const int SIZE = 4096;
	const char *name = "OS";

        // Declares the shared memory space and the pointer to the shared memory space
	int shm_fd;
	void *ptr;
  
        // Initializes the shared memory with the name and adds the functionality to read and write from the shared memory
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  
        // Sets the size of the shared memory to the SIZE initialized at the beginning
	ftruncate(shm_fd, SIZE);
  
        // Maps the shared memory space to the virtual address space
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	// If the map fails, then terminate program
	if(ptr == MAP_FAILED){
		printf("Map Failed\n");
		return -1;
	}

        // Start forking process
	pid = fork();
  
        // If the pid is less than 0, then the fork has failed, so terminate the program
        if(pid < 0){
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if(pid > 0){  // parent process
	        // wait for child process to finish
		wait(NULL);
  
                // Fetches value of child_current from the shared memory segment
		memcpy(&child_current, ptr, sizeof(struct timeval));
  
                // Gets the current time and stores it in parent_current
		gettimeofday(&parent_current, NULL);
  
                // subtract child's current time from parent's, then print
		timersub(&parent_current, &child_current, &time_elapsed);
		printf("Elapsed time: %ld.%ld\n", time_elapsed.tv_sec, time_elapsed.tv_usec);
      
                // unlink shared memory
		shm_unlink(name);
	}
	else{ 
	        // Gets the current time and stores it in child_current
		gettimeofday(&child_current, NULL);
		// Writes the value of child_current into the shared memory segment
		memcpy(ptr, &child_current, sizeof(struct timeval));
      
                // executes command
		execvp(argv[1], &argv[1]);
	}
	return 0;
}
