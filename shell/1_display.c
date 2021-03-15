#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include<stdlib.h>
#include<limits.h>
#include "Functions.h"
#include <sys/utsname.h>
#define N 1001
#define M 200
void prompt()
{
	char *buf,*words,*uname,hostname[N];
	uname=getlogin();
	int hostname_file=open("/etc/hostname",O_RDONLY);
	read(hostname_file,hostname,N*sizeof(char)+1);
	int i,j;for(i=0;i<N;i++)if(hostname[i]==10){hostname[i]=0;break;}
	printf("<%s@%s:",uname,hostname);
	buf=getcwd(NULL,0);
	char **levels=parse(buf,'/',1);
	i=0;while(*(levels+i))i++;
	if(i>=2&&strcmp(*(levels),"home")==0&&strcmp(*(levels+1),uname)==0)
	{
		printf("~");j=2;
		while(*(levels+j))
			printf("/%s",*(levels+j++));
	}
	else
	{
		j=0;
		while(*(levels+j))
			printf("/%s",*(levels+j++));
	}
	clrptr(levels);
	printf(">");
}
void storeBG()
{

	int i;extern int *BGP;
	for(i=0;i<1000;i++)//Max 1000 background processes allowed
	{
		*(BGP+i)=-1;
	}
}
void printBG()
{
	extern int *BGP;
	int i;
	for(i=0;i<1000;i++)
	{
		if(*(BGP+i)!=-1)
			printf("[%d] %d\n",i+1,*(BGP+i));
	}
}
void overkill()
{
	extern int *BGP;
	int i;
	for(i=0;i<1000;i++)
	{
		if(*(BGP+i)!=-1)
			kill(*(BGP+i),SIGKILL);
	}
}
void addBGP(pid_t Pid)
{
	int i=0;extern int *BGP;
	while(*(BGP+i)!=-1)i++;
	*(BGP+i)=Pid;
}
void Done()
{
	int i=0;extern int *BGP;
	for(i=0;i<1000;i++)
	{
		if(*(BGP+i)!=-1)
		{
			pid_t Pid = waitpid(*(BGP+i), NULL, WNOHANG);
			if(Pid==*(BGP+i))
			{
				fprintf(stderr,"[-] Done : %d\n",Pid);
				*(BGP+i)=-1;
			}
		}
	}
}
void kjob(int jobno,int signo)
{
	extern int *BGP;	
	kill(*(BGP+jobno-1),signo);
}