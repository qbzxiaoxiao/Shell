#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<signal.h>
#include<sys/types.h>
#include<pwd.h>
#include<sys/utsname.h>
#define STRLEN 128
char OLDPWD[STRLEN]={0};
//输出提示符信息
void OutPutFlag()
{
	char flag='$';
    struct passwd *pw=getpwuid(getuid());//用户id
	assert(pw!=NULL);
	if(pw->pw_uid==0)//超级管理员
	{
		flag='#';
	}
	struct utsname hostname;//获取内核信息
	uname(&hostname);//主机名
	char node[STRLEN]={0};
	strcpy(node,hostname.nodename);
	char *name=strtok(node,".");

	char path[STRLEN]={0};
	getcwd(path,STRLEN-1);//获取当前工作目录的绝对路径
	char *p=path+strlen(path);
	while(*p!='/')
	{
		p--;
	}
	if(strlen(path)!=1)//==1时在根目录只有/
	{
    	p++;//在++之前P指向/
	}
	if(strcmp(path,pw->pw_dir)==0)
	{
		p="~";
	}
	printf("mybash[%s@%s %s]%c ",pw->pw_name,name,p,flag);
	fflush(stdout);

}


void MyCd(char *path)
{
	if(path==NULL)
	{
		return ;
	}
	if(strcmp(path,"~")==0)
	{
		struct passwd *pw=getpwuid(getuid());
		path=pw->pw_dir;

	}
	if(strcmp(path,"-")==0)
	{
		if(strlen(OLDPWD)==0)
		{
			printf("mybash: cd: OLDPWD not set\n");
			return ;
		}
		//path=OLDPWD;
		strcpy(path,OLDPWD);

	}
	char oldpwd[STRLEN]={0};
	getcwd(oldpwd,STRLEN-1);
    if(-1==chdir(path))//改变当前工作目录
	{
		char err[STRLEN]="mybash: cd: ";
		strcat(err,path);
		perror(err);
		return ;
	}
    strcpy(OLDPWD,oldpwd);
}
void Zombie()
{
	wait(NULL);
}

void IncisionString(char *cmd,char *argv[])
{
	int count=0;
	char *p=strtok(cmd," ");
	while(p!=NULL)
	{
		argv[count++]=p;
		p=strtok(NULL," ");
	}

}
int main()
{
    signal(SIGCHLD,Zombie);//后台运行 当子进程结束时父进程获取子进程退出码 处理子进程僵死
	while(1)
	{
		OutPutFlag();
		char cmd[STRLEN]={0};
		fgets(cmd,STRLEN,stdin);
		cmd[strlen(cmd)-1]=0;

		int black=0; //判断前台还是后台运行 0-前台  1-后台
		if(strstr(cmd,"&")!=NULL)//strstr()
		{
			black=1;
		}
		char *argv[STRLEN]={0};

        IncisionString(cmd,argv);//切割字符串
        if(argv[0]==NULL)
		{
			continue;
		}
        
		if(strncmp(argv[0],"cd",2)==0)///////
		{
			MyCd(argv[1]);
			continue;
		}
		if(strncmp(argv[0],"exit",4)==0)///////
		{
			exit(0);
		}
	    pid_t n=fork();
		assert(n!=-1);

		if(0==n)//子进程
		{
			char path[STRLEN]={"/home/xiaoxiao/Desktop/shell/mybin/"};//////

			if(strstr(argv[0],"/")!=NULL)
			{
				strcpy(path,argv[0]);
			}
			else
			{
			    strcat(path,argv[0]);
			}
			execv(path,argv);
			printf("bash: %s: Command Not Found\n",argv[0]);
			exit(0);//防止子进程继续运行			
		}
		else
		{
			if(!black)
			{
			   pause();//接收信号才返回，否则阻塞   
			}	
		}
	}
	return 0;
}
