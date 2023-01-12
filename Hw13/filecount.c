#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int pid1, pid2;
void signal_ctr_c(int signum) {
    //殺掉process group   
    fprintf(stderr, "kill process group %d\n", -1*pid1);
    kill(-1*pid1, signum);
    //parent結束離開
    _exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    int pipefd[2];
    int ret, wstat;

    if (argc != 2){
        printf("usage: ./filecount /YYY\n");
        exit(EXIT_FAILURE);
   	}
    //char **param={"EXENAME", NULL};
    printf("parent's group id is %d\n", getpgrp());
    pipe(pipefd);
    
	pid1 = fork();   		//產生第一個child
    if (pid1 == 0) {
        printf("1st child's group id is %d\n", getpgrp());
        //setpgid(0, 0);  	//將第一個child設定為新的group
        printf("1st child's new group id is %d\n", getpgrp());
        close(1);   		//關閉stdout
        dup(pipefd[1]); 	//將pipefd[1]複製到stdout
        close(pipefd[1]);   //將沒用到的關閉
        close(pipefd[0]);   //將沒用到的關閉
        execlp("ls","ls","-R",argv[1], NULL);	//ls會將東西藉由stdout輸出到pipefd[1]
        //execlp("sleep", "sleep", "100s", NULL);//睡100sec
    }
	else 
		printf("1st child's pid = %d\n", pid1);
    
	if (pid1 > 0) {    
        pid2 = fork();//產生第二個child
        if (pid2==0) {
            printf("2nd child's group id is %d\n", getpgrp());
            //setpgid(0, pid1);   //第二個child加入第一個child的group
            printf("2nd child's new group id is %d\n", getpgrp());
            close(0);   //關閉stdin
            dup(pipefd[0]); //將pipefd[0]複製到stdin
            close(pipefd[1]);   //將沒用到的關閉
            close(pipefd[0]);   //將沒用到的關閉
            execlp("wc","wc","-l", NULL);   //執行wc，wc將透過stdin從pipefd[0]讀入資料
        } 
		else 
			printf("2nd child's pid = %d\n", pid2);
    }
    //parent一定要記得關掉pipe不然wc不會結束（因為沒有接到EOF）
    close(pipefd[0]);
	close(pipefd[1]);
    
	/*parent註冊signal handler*/
    signal(SIGINT, signal_ctr_c);
    printf("child %d\n",wait(&wstat));
    printf("child %d\n",wait(&wstat));
}
