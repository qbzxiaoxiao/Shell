#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<time.h>
#include<grp.h>
#include<stdlib.h>
#include<pwd.h>

char CurrentPath[128];
int Option=0;
#define IS_A 1
#define IS_L 12
#define IS_I 9

#define IS_OPTION(option,flag)  ((option)&(1<<((flag)-1)))

//寻找路径并解析选项部分
void AnayPara(char *argv[],int argc,char *path[])
{
	int i=1;
	int count=0;
	for(;i<argc;i++)
	{
		if(strncmp(argv[i],"-",1)!=0)//路径
		{
			path[count++]=argv[i];
			continue;

		}
		char *p=argv[i]+1;//p-->第一个有效选项-a -i -l
		while(*p!=0)
		{
			Option|=1<<(*p-'a');
			p++;
		}

	}

	if(count==0)//用户没有给定路径
	{
		getcwd(CurrentPath,127);
		path[0]=CurrentPath;
	}
}

void ModeLetter(int mode,char *str)
{
	strcpy(str,"----------.");
	if(S_ISDIR(mode))//目录文件
	{
		str[0]='d';
	}
	if(S_ISFIFO(mode))//管道文件
	{
		str[0]='p';
	}
	if(S_ISLNK(mode))//链接文件
	{
		str[0]='l';
	}
	if(S_ISBLK(mode))//块设备
	{
		str[0]='b';
	}
	if(S_ISCHR(mode))//字符设备
	{
		str[0]='c';
	}
	if(S_ISSOCK(mode))//套接字
	{
		str[0]='s';
	}
    
    if(mode & S_IRUSR)
	{
      str[1]='r';
	}
	if(mode & S_IWUSR)
	{
		str[2]='w';
	}
	if(mode & S_IXUSR)
	{
		str[3]='x';
	}
	
    if(mode & S_IRGRP)
	{
      str[4]='r';
	}
	if(mode & S_IWGRP)
	{
		str[5]='w';
	}
	if(mode & S_IXGRP)
	{
		str[6]='x';
	}

	
    if(mode & S_IROTH)
	{
      str[7]='r';
	}
	if(mode & S_IWOTH)
	{
		str[8]='w';
	}
	if(mode & S_IXOTH)
	{
		str[9]='x';
	}
}

void Is_L(int flag)
{
	if(flag==1)
	{
      printf("\n");
	}
}


void PrintFileInfo(struct dirent *d,char *path)
{
	if(!IS_OPTION(Option,IS_A)&&strncmp(d->d_name,".",1)==0)//ls -a无 .和 ..
	{
		return ;
	}
	int lflag=0;
	struct stat st;
	memset(&st,0,sizeof(st));
	char pathname[128]={0};
	strcpy(pathname,path);
	strcat(pathname,"/");
	strcat(pathname,d->d_name);
   
	int res=lstat(pathname,&st);//获取文件信息
    assert(res!=-1);

	if(IS_OPTION(Option,IS_I))
	{
		printf("%d  ",d->d_ino);
	}
	if(IS_OPTION(Option,IS_L))
	{
 	  lflag=1;
      char str[12];
      ModeLetter(st.st_mode,str);
	  
	  struct passwd *pw=getpwuid(st.st_uid);
	  assert(pw!=NULL);
	  
	  struct group *grp=getgrgid(st.st_gid);
	  assert(grp!=NULL);
     
	  printf("%s ",str);
	  printf("%d ",st.st_nlink);
	  printf("%10s ",pw->pw_name);
	  printf("%10s ",grp->gr_name);
	  printf("%6d ",st.st_size);
	  printf("%.12s ",ctime(&st.st_mtime)+4);
	     
	}

    if(S_ISDIR(st.st_mode))
	{
		printf("\033[1;34m%s\033[0m   ",d->d_name);	
		if(lflag==1)
		{
			printf("\n");
		}
		return ;
	}
	if(st.st_mode & S_IXUSR || st.st_mode &S_IXGRP || st.st_mode & S_IXOTH)
	{
		printf("\033[1;32m%s\033[0m    ",d->d_name);	
		if(lflag==1)
		{
			printf("\n");
		}
		return;
	}
	printf("%s  ",d->d_name);
	if(lflag==1)
	{
		printf("\n");
	}
}



int main(int argc,char *argv[])
{
	char *path[128]={0};
	AnayPara(argv,argc,path);

	int i=0;
	while(path[i]!=NULL)
	{
		DIR *dirp=opendir(path[i++]);//获取目录下的所有文件列表
		if(dirp==NULL)
		{
           printf("%sNot fond\n",path[i-1]);
		   break;
		}
	
	    struct dirent *d=NULL;
		
		while((d=readdir(dirp))!=NULL)//读取文件列表有 . 和..这两个个
		{
			PrintFileInfo(d,path[i-1]);
          	
		}
		if(!IS_OPTION(Option,IS_L))
		{
			printf("\n");
		}	
		closedir(dirp);

	}
	return 0;

}
