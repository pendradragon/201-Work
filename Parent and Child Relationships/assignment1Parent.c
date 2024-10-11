#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	pid_t child;

	//creating the child function using fork
	child = fork();

	//fork failed case
	if (child < 0){
		perror("Fork failed");
		printf("The fork failed.");
		exit(1);
	}

	//the fork actually works -- has to follow the rules given in the assignment
	else if(child == 0){
		//the child function is defined in a separate file that is located in the same folder. The file that has to be used is ass1Child.c
		printf("The child process (PID: %d) was created by the parent process (PID: %d).\n", getpid(), getppid()); //ID printing
		
		//hardcoding in the file name for the child program executable
		char *path[] = {"./ass1ChildEX", NULL};
		
		//executing the child program using "exec"
		execv(path[0], path);
		
		//if the executing fails
		perror("execv failed.\n");
		exit(1);
	}


	//the stuff the parent process has to do
	else{
		printf("==== Parent Process (PID: %d) ====\n", getpid()); //header
		printf("The process successfully created the child process (PID: %d)", child); //printing the ID of the chlid
		wait(NULL); //waiting for the child process to finish
		
		//needs to print a function that indicates when the parent is finished working
		printf("The parent process (this program) detected that the child process (PID: %d) finished running.\n", child);
		printf("The parent process (PID: %d) has finished running.\n", getpid());
	}
	
	return 0;
}

	
