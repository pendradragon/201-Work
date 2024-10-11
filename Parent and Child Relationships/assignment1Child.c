#include <stdio.h>
#include <unistd.h>

int main(){
	//things the child program has to do
	printf("==== Child Process (PID: %d) ====\n", getpid()); //header
	
	//putting the child to sleep -- it's nap time
	printf("The child process is going to sleep for 120 seconds.\n");
	sleep(120);
	
	//printing a message after waking up --- the program will be done running after the program wakes up
	printf("The child program has waken up and has finsihed running.\n");

	return(0);
}
