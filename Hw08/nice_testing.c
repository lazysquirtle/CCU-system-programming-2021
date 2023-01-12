#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

long long int cpp = 0;
int pid = 0;

void alarm_handler(int signo){
	if(pid > 0)
		printf("parent : ");
	else
		printf("child  : ");
	printf("cpp = %lld\n", cpp);
	exit(EXIT_SUCCESS);
}


int main(int argc,char* argv[]){
	if (argc < 2){
		printf("lack of arguments\n");
		exit(EXIT_FAILURE);
	}	

	int nice_value = atoi(argv[1]);	//read the nice value 
	pid = fork();
	if(pid > 0)						//parent
		nice(nice_value);
		
	
	signal(SIGALRM,alarm_handler);
	alarm(1);
	
	
	while(1)
		cpp++;	

	return 0;
}
