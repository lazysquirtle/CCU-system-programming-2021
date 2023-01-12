#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>
#include <locale.h>
#include <wchar.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/limits.h>

//file type array
int  file_type[20];
char file_symbol[20];

long total_file_num = 0;

//initialize the file type array , if X exits,then file_type[X]=1 
void InitFileType(){
	for(int i = 0 ; i < 19 ; i++)
		file_type[i] = -1;

	file_symbol[DT_BLK] = 'b'; 		//block device 
	file_symbol[DT_CHR] = 'c'; 		//character device 
	file_symbol[DT_DIR] = 'd'; 		//directory 
	file_symbol[DT_FIFO]= 'f'; 		//fifo
	file_symbol[DT_LNK] = 'l'; 		//symbolic link
	file_symbol[DT_REG] = '-'; 		//regular file 
	file_symbol[DT_SOCK]= 's'; 		//socket
	file_symbol[DT_UNKNOWN] = 'u';

}

int ReadSize(char* pathname){
	struct stat buf;
	printf("read size: %s\n",pathname);
	assert(stat(pathname,&buf)==0);
	return buf.st_size;
}

long CountDirectory(char* path){
	
	long size = 0; 
	DIR* dirptr = opendir(path);
	struct dirent* ent = readdir(dirptr);
	
	while(ent != NULL){
		if(strcmp(ent->d_name , ".")==0 || (strcmp(ent->d_name,"..")==0) ){
			ent = readdir(dirptr);
			continue;
		}
		
		file_type[ent->d_type] = 1;
		
		char pathname[PATH_MAX]="";
		strcat(pathname,path);
		strcat(pathname,"/");
		strcat(pathname,ent->d_name);
		printf("%s\n",pathname);
		
		if(ent->d_type == DT_REG)
			size += ReadSize(pathname);
		else if(ent->d_type == DT_DIR){
			printf("Enter CountDirectory : %s\n",pathname);
			size += CountDirectory(pathname);	
		}
		ent = readdir(dirptr);
		total_file_num++;
	}

	closedir(dirptr);
	return size;
	
}


int main (int argc, char* argv[]){
  	InitFileType();
	long size = 0;
	size = CountDirectory(argv[1]);

	/*
	printf("授課老師羅習五的生日是:1990/04/10\n");
	printf("總共大小:%ld bytes\n",size);
	printf("檔案種類 -\n");
	printf("檔案含子目錄內總共有%6ld\n",total_file_num);
	*/
	
	setlocale(LC_ALL,"");
	wchar_t wstr0[] = L"授課老師羅習五的生日是:1990/04/10";
	wchar_t wstr1[] = L"總共大小:" ;
	wchar_t wstr2[] = L"檔案種類 -";
	wchar_t wstr3[] = L"檔案含子目錄總共有";
	printf("%ls\n",wstr0);
	printf("%ls bytes\n",wstr1);
	printf("%ls\n",wstr2);
	printf("%ls %6ld\n",wstr3,total_file_num);

	//wprintf(L"%ls\n",wstr);
	
	return 0;
}
