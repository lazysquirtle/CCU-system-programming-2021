#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define NUM 10

void child_task(void){
	char *arg[] = {"ls","-alh","/",NULL};
	printf("I am child,my pid is %d\n",getpid());
	execve("/usr/bin/ls",arg,NULL);
	perror("execve fail : ");
	exit(EXIT_FAILURE);
}

int main (int argc,char* argv[]){
	pid_t pid;
	int i,status;
	for (i = 0 ;i < NUM ;i++ ){
		pid = vfork();
		if (pid < 0)
			perror("fork fail :");
		else if (pid == 0)
			child_task();
		else
			continue;		

	}
	
	if(pid > 0)
		getchar(); 
	return 0;
}
