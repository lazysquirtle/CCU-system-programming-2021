#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM 100

void child_task(void){
	int pid,wait_status;
	for(int i = 1 ;i <= NUM ;i++ ){
		pid = fork();
		if(pid < 0)
			perror("fork fail:");
		else if(pid == 0){
			printf("(child) My pid is %-4d. ",getpid());
			printf("(sleep) about %d seconds\n",i);
			sleep(i);
			printf("(child) My pid is %-4d. ",getpid());
			printf("(wake up) after %d seconds\n",i);

			printf("By the way,my father is %d\n",getppid());
			exit(EXIT_SUCCESS);
		}
		else
			continue;				
	}
}

int main(int argc,char* argv[]){
	pid_t pid;
	int status;

	pid = fork();
	if(pid == 0){
		child_task();		
		exit(EXIT_SUCCESS);
	}
	wait(&status);	 
	getchar();	
	return 0;
}
