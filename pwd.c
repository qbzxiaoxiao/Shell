#include<stdio.h>
#include<unistd.h>

#define STRLEN 128

int main()
{
	char path[STRLEN]={0};
	getcwd(path,STRLEN-1);
	printf("%s\n",path);

	return 0;
    
}
