#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/file.h>

char buffer[20];
char accept[20];

int main (int argc,char* argv[]){
	int fd;
	int check_read;
	fd = open("lockf.db",O_RDWR);
	if (fd < 0){
		perror("open failed ");
		exit(EXIT_FAILURE);
	}

	printf("lockf-------?\n ");
	if (lockf(fd,F_LOCK,0)<0){
		perror("lockf error ");
		exit(EXIT_FAILURE);
	}
	printf("lockf success\n");

	check_read = read(fd,buffer,20);
	if(check_read == 0){
		perror("read fail ");
		exit(EXIT_FAILURE);
	}

	int now = 0;
	int num = atoi(buffer);
	int len = strlen(buffer);
	int i = 0;

	while(i<1000){						//execute 1000 times 
		usleep(100000);  				//wait 0.1 second 	
		printf("count  :%d\n",i+1);
		now = lseek(fd,0,SEEK_CUR);		//get current fptr
		printf("Now at :%d\n",now);
		now = lseek(fd,num,SEEK_END);	//add X offsets
		
		sprintf(accept,"%d",++num);		//store X+1
		len = strlen(accept);			//for sure no change in num length
		write(fd,accept,len); 			//write to the file 
		i++;
	}	

	return 0;
}
