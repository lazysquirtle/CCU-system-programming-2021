//Reference 
//https://wirelessr.gitbooks.io/working-life/content/wait_vs_waitpid.html
//https://blog.csdn.net/solarlhh/article/details/10088957

//Thanks to 
//習五老師
//好友博禕

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>
#include <grp.h>
#include <pwd.h>

char* ltrim(char* s){
	while(isspace(*s))
		s++;
	return s;
}

char* rtrim(char* s){
	char* back = s + strlen(s);
	while(isspace(*--back));
	*(back+1) = '\0';
	return s;
}

char* trim(char* s){
	return rtrim(ltrim(s));
}

void main(int argc ,char* argv[]){
	char  username[1024];
	char* name_ptr;
	
	//<limits.h>
	long ngroups_max;
	char buffer[1024];
	char password_str[sysconf(_SC_PASS_MAX)];
	
	struct passwd passwd_ent;
	struct passwd *result;
	struct group  *gr;

	gid_t gid;
	gid_t groups[sysconf(_SC_NGROUPS_MAX)];
	
	int ret;
	int nGroup = sysconf(_SC_NGROUPS_MAX);	
	
	relogin:
		printf("Please enter the name :\n");
		//assert(fgets(username, 1024, stdin) != NULL);
		name_ptr = fgets(username ,1024 ,stdin);
		printf("gets :%s\n",name_ptr);
		
		//取消密碼功能
		//printf("Please enter the password\n");
		//strncpy(password_str,getpass("Password:"),sysconf(_SC_PASS_MAX));
		
		//trim the non-ascii symbol at head/tail of the string
		name_ptr = trim(name_ptr);
		
		//check if the users are authorized by the system
		ret = getpwnam_r(name_ptr ,&passwd_ent ,buffer ,1024 ,&result);
		if(result == NULL)
		{
			fprintf(stderr,"你輸入的使用者不在系統中\n");
			goto relogin;
		}			
	 
		pid_t pid = fork();
		if (pid > 0){					//爺爺行程
			int wait_status;
			wait(&wait_status);
			goto relogin;
		} 
		else{							//媽媽or爸爸行程
			//讓fork()出來的child睡一下,參考reference看到的
			sleep(1);
			//查詢這個使用者屬於哪些group
         	ret = getgrouplist(name_ptr ,passwd_ent.pw_gid ,groups ,&nGroup);
          	printf("GetGroupList = %d\n",ret);
          	printf("User ID   :%d\n",passwd_ent.pw_uid);
          	printf("User Name :%s\n",passwd_ent.pw_name);
          	printf("Group ID  :%d\n",passwd_ent.pw_gid);
          	printf("Home Dir  :%s\n",passwd_ent.pw_dir);
          	printf("Other Msg :%s\n",buffer);
         	printf("Total Groups as follow : ");
         	printf("Num = %d\n ",nGroup);
         	for(int i = 0 ;i < nGroup ;i++ ){
             	gr = getgrgid(groups[i]);
             	printf("%s, ",gr->gr_name);
         	}
         	printf("\n");

			//先設定使用者的gid
         	assert(setgid(passwd_ent.pw_gid)==0);
         	//改變工作目錄，避免使用者一開始的目錄不是家目錄，讓剛進入的使用者感覺很怪
         	assert(chdir(passwd_ent.pw_dir)==0);
         	//改變環境變數"HOME",overwrite value = 1
         	setenv("HOME",passwd_ent.pw_dir,1);    
         	//改變使用者的UNIX群組,(0,NULL) => drop all the supplementary groups
         	setgroups(0,NULL);
         	//改變使用者的擴充群組
         	setgroups(sysconf(_SC_NGROUPS_MAX),groups);
 
			//上述的權限設定可以成功是因為目前uid還是root
			//下一行執行下去以後，就無法改變權限設定了
         	assert(setuid(passwd_ent.pw_uid)==0);

			printf("New bash\n");
			//execute the users' shell
			pid_t new_pid = fork();			
			if (new_pid > 0){
				int wait_stat = -1;
				pid_t child_pid = waitpid(-1,&wait_stat,0);
				assert(child_pid != -1);
			}
			else{
				sleep(1);
				char* args[] = {"bash",NULL};
				execvp(args[0],args);
				exit(EXIT_SUCCESS);
			}

			printf("base的回傳值是%d\n",ret);
			exit(EXIT_SUCCESS);

		}
		
 

}


