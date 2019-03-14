#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<pwd.h>
#include<shadow.h>
#include<stdlib.h>
#include<assert.h>
#include<termios.h>
int main(int argc,char *argv[])
{
  char *user="root";
  if(argc>1)
  {
	  user=argv[1];
  }

  printf("Password:");
  fflush(stdout);
  struct termios oldtm,newtm;
  tcgetattr(0,&oldtm);
  newtm=oldtm;
  newtm.c_lflag &=~ECHO;
  newtm.c_lflag &=~ICANON;
  tcsetattr(0,TCSANOW,&newtm);

  char passwd[128]={0};
  int i=0;
  char c=0;
  while((c=getchar())!='\n')
  {
	 
	  if(c==127)
	  {
		  if(i<=0)
		  {
			  continue;
		  }
		  passwd[--i]=0;
          printf("\033[1D");
          printf("\033[K");
		  continue;
	  }
	  passwd[i++]=c;
	  printf("*");
	  fflush(stdout);
  }
 
  tcsetattr(0,TCSANOW,&oldtm);
  printf("\n");

  struct spwd *sp=getspnam(user);//获取用户和组的信息
  assert(sp!=NULL);
  char salt[128]={0};
  char *p=sp->sp_pwdp;
  int count=0;
  int j=0;
  while(*p!=0)
  {
	  salt[j++]=*p;
	  if(*p=='$')
	  {
		  count++;
	  }
	  if(count==3)
	  {
		  break;
	  }
	  p++;
  }

   char *pwdp=crypt(passwd,salt);//匹配密码
   if(strcmp(pwdp,sp->sp_pwdp)!=0)
   {
	   printf("Password is error\n");
	   exit(0);
   }

   pid_t n=fork();
   assert(n!=-1);

   if(n==0)
   {
	   struct passwd *pwd=getpwnam(user);
	   assert(pwd !=NULL);

	   setuid(pwd->pw_uid);
	   setenv("HOME",pwd->pw_dir,1);

	   execl(pwd->pw_shell,pwd->pw_shell,(char *)0);

	   printf("su exec lail\n");
	   exit(0);

   }
    else
	{
		wait(NULL);
	}

   

	return 0;
}



