#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>
#include<fcntl.h>

#define STRLEN 128
void getAbspath(char *path,char* abspath)
{
	char curpath[128]={0};
    getcwd(curpath,STRLEN-1);//当前路径无/
	if(strncmp(path,"../",3)==0)
	{
		char *pcur=curpath+strlen(curpath)-1;
		while(*pcur!='/')
		{
			*pcur='\0';
			pcur--;
		}
		strcat(curpath,path+3);
	//	printf("%s\n",path+3);
        strcpy(abspath,curpath);
	}
	else if(strncmp(path,"./",2)==0)
	{
      strcat(curpath,path+1);
	  strcpy(abspath,curpath);
	}
	else if(strncmp(path,"/",1)==0)
	{
		strcpy(abspath,path);

	}
	else 
	{
		strcat(curpath,"/");
		strcat(curpath,path);
		strcpy(abspath,curpath);
	}
}
void Delete(char* input,char *abspath)
{
	if(abspath==NULL)
	{
		return ;
	}

	char path[STRLEN]={0};
    strcpy(path,abspath);

	char *pcur=path;
    int count=0;//保存/的个数
	while(*pcur!='\0')
	{
		if(*pcur=='/')
		{
			count++;
		}
		pcur++;
	}

	if(count<=1)//只有一个/或/文件名
	{
      printf("touch: setting time of '%s': Permission denied\n",input);
	  return ;
	}
   while(*pcur!='/')
   {
	   *pcur='\0';
	   pcur--;
   }
	DIR* dr=opendir(path);
	if(dr==NULL)
	{
      printf("touch: cannot touch '%s': No such file or directory\n",input);
	  return ;
	}

	int fd=open(abspath,O_RDONLY|O_CREAT,0664);
    if(fd==-1)
	{
      printf("touch: cannot touch '%s': No such file or directory\n",input);
	  return ;
	}
	close(fd);

}
int main(int argc,char *argv[])
{

	 if(argc < 2)
	 {
		 return 0;
	 }
     char abspath[STRLEN]={0};
	 int i=1;
	 for(;i<argc;i++)
	 {
		 getAbspath(argv[i],abspath);
     
		 Delete(argv[i],abspath);
      
		 memset(abspath,0,STRLEN);

	 }
     

	return 0;


}
