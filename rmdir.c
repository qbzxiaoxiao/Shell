#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>

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
    
	DIR* dr=opendir(abspath);//判断此目录是否存在
	if(dr==NULL)
	{
		printf("rmdir: failed to remove '%s':No such file or directory\n",input);
		return ;
	}
   struct dirent *d=NULL;
   while((d=readdir(dr))!=NULL)//判断此目录是否为空
   {
	   if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0)
	   {
		   printf("rmdir: failed to remove '%s':Directory not empty\n",input);
		   return ;
	   }
   }
   closedir(dr);
   if(rmdir(abspath)==-1)
   {
      printf("rmdir: failed to remove '%s':Permission denied\n",input);
   }

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
