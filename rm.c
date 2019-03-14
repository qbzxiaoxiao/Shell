#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>
#include<assert.h>

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
int  IsTruePath(char* abspath,char* endname)//正确返回1，否则返回0
{
	if(abspath==NULL)
	{
		return 0;
	}
	char path[STRLEN]={0};
	char end[STRLEN]={0};
	int count=0;
	strcpy(path,abspath);

	char *pcur=path+strlen(path)-1;

	while(*pcur!='/')
	{
		end[count++]=*pcur;
		*pcur='\0';
		pcur--;
	}
    end[count]='\0';
    *pcur='\0';

	count=0;
	int len=strlen(end);
	for(;count<len/2;count++)
	{
		char tmp=end[count];
		end[count]=end[len-1-count];
		end[len-1-count]=tmp;
	}
                                   //路径最下一层的文件名或目录名

	DIR* dr=opendir(path);//判断最下一层目录是否存在
	if(dr==NULL)
	{			     
		return 0;
	}
   struct dirent* d=NULL;
   while((d=readdir(dr))!=NULL)//判断此目录中是否有此文件或目录
   {
	   if(strcmp(d->d_name,end)==0)//
	   {
		  strcpy(abspath,path);
          strcpy(endname,end);
          closedir(dr);
		return 1;
	   }
    }
   closedir(dr);
   return 0;
}
void DeleteDir(char* abspath)//删除目录文件及其目录下所以文件
{
	DIR* dr=opendir(abspath);
	struct dirent* d=NULL;
	struct stat st;
	char curpath[STRLEN]={0};

	while((d=readdir(dr))!=NULL)
	{
	
		if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0)
		{	
		   memset(curpath,0,STRLEN);
           memset(&st,0,sizeof(st));

		   strcpy(curpath,abspath);
		   strcat(curpath,"/");
		   strcat(curpath,d->d_name);
           int res=lstat(curpath,&st);
           assert(res!=-1);

          if(S_ISDIR(st.st_mode))//是目录文件
		  {		  
              DeleteDir(curpath);		
			  rmdir(curpath);
     	  }
		  else
		  {
			  unlink(curpath);
		  }

		}
	}
}
void DeleteHaveR(char* abspath,char* endname)
{
	DIR* dr=opendir(abspath);
	struct dirent* d=NULL;
	struct stat st;
	memset(&st,0,sizeof(st));

    strcat(abspath,"/");
	strcat(abspath,endname);

	int  res=lstat(abspath,&st);
	assert(res!=-1);
    
	while((d=readdir(dr))!=NULL)
    {
		if(strcmp(d->d_name,endname)==0)
		{      
           if(S_ISDIR(st.st_mode))//是目录文件
		   {      
            DeleteDir(abspath);	
			rmdir(abspath);
		   } 
		   else
		   {
              unlink(abspath);
		   }
		   
		}
	}

}
int  DeleteNoR(char* abspath,char*endname)
{
	int flag=0;

	DIR* dr=opendir(abspath);
	struct dirent* d=NULL;
	struct stat st;
	memset(&st,0,sizeof(st));

    strcat(abspath,"/");
	strcat(abspath,endname);

	int  res=lstat(abspath,&st);
	assert(res!=-1);
    
	while((d=readdir(dr))!=NULL)
    {
		if(strcmp(d->d_name,endname)==0)
		{      
           if(S_ISREG(st.st_mode))//是普通文件
		   {
			   flag=1;
			   unlink(abspath);			 	   
		   }
		   else
		   {
			   flag=0;		     
	       }
          break;
		}
    }
	return flag;
}

int main(int argc,char *argv[])
{
     char abspath[STRLEN]={0};
     char endname[STRLEN]={0};
	 if(argc < 2)
	 {
		 printf("rm: missing operand\n");
		 return 0;
	 }
	 if(strcmp(argv[1],"-r")==0)
	 {
		 if(argc<3)
		 {
			 printf("rm: missing operand\n");
		 }
        int i=2;
	
	   for(;i<argc;i++)
	   {
		 getAbspath(argv[i],abspath);
     
		 if(IsTruePath(abspath,endname)==0)
		 { 
			 printf("rm: failed to remove '%s': No such file or directory\n",argv[i]);      
	     }
		 else//此时abspath已经变为之前abspath的上一级目录
		 {
			 DeleteHaveR(abspath,endname);

		 }
		
	     memset(endname,0,STRLEN);
		 memset(abspath,0,STRLEN);

	   }
	 }
	 else
	 {
	    int i=1;
	   for(;i<argc;i++)
	   {
		 getAbspath(argv[i],abspath);
     
		 if(IsTruePath(abspath,endname)==0)
		 { 
			 printf("rm: failed to remove '%s': No such file or directory\n",argv[i]);      
	     }
        else
		{
	      if(DeleteNoR(abspath,endname)==0)
		  {
			  printf("rm: cannot remove '%s': Is not a Common file\n ",argv[i]);

		  }
		}


		 memset(endname,0,STRLEN);
		 memset(abspath,0,STRLEN);

       }
	 }

	return 0;
}
