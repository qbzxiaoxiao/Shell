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
		//printf("%s\n",path+3);
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
void Creat(char* input,char *abspath,int pos)
{
	if(abspath==NULL|| pos>=strlen(abspath))
	{
		return ;
	}

	
//	printf("len: %d\n",pos);


    char path[STRLEN]={0};
	int count=0;
	char *pcur=(abspath+pos);
	if(pos==0)
	{
		path[count++]='/';
	}
	else
	{
		memcpy(path,abspath,pos+1);
		count=pos+1;
		pcur++;
	}
	//printf("kkk:%s\n",path);
	
    while(*pcur!='\0' && *pcur!='/')
	{
		path[count++]=*pcur;
		pcur++;
	}
    path[count]='\0';


	//printf("Creat: %s\n",path);
//	printf("len+count:%d\n",count);
//	printf("-------------------------------------\n");
    DIR* dr= opendir(path);
	if(dr==NULL)
	{
     if(mkdir(path,0777)==-1)
	 {
      printf("mkdir: failed to Creat '%s':Permission denied",input);
	  return ;
	 }
	}
   
    Creat(input,abspath,count);

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
	//	 printf("main:argv[i]:   %s\n",argv[i]);
		 getAbspath(argv[i],abspath);
     //    printf("main:abspath:   %s\n",abspath);
		 Creat(argv[i],abspath,0);
      // Creat("/home/xiaoxiao",0);

		 memset(abspath,0,STRLEN);

	 }
     

	return 0;


}
