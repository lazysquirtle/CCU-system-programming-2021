#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>

int main(int argc,char* argv[]){
	char  exebuf[100];
	char* exename = basename(argv[0]);
		
	if (strcmp(exename,"ls")==0 && argc == 2)
		sprintf(exebuf,"%s %s",exename,argv[1]);
	else if (strcmp(exename,"cp" )==0 && argc==3)
		sprintf(exebuf,"%s %s %s",exename,argv[1],argv[2]);
	else if (strcmp(exename,"cat")==0 && argc==2)
		sprintf(exebuf,"%s %s",exename,argv[1]);
	else if (strcmp(exename,"chown")==0 && argc==3)
		sprintf(exebuf,"%s %s %s",exename,argv[1],argv[2]);
	else{
		printf("Nothing Match\n");
		exit(EXIT_FAILURE);
	}	
	system(exebuf);


	
}
