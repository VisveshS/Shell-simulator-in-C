#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#include<limits.h>
#include <sys/utsname.h>
#include "Functions.h"
#define N 1001
char *input()
{
	char *line=NULL;
	ssize_t bufsize = 0;
	getline(&line,&bufsize,stdin);
	line[strlen(line)-1]=0;
	return line;
}
char *compress(char *line)
{
	int space=1,i,flag=0,k=0,enable=1;
	char *Line=(char*)malloc(sizeof(char)*strlen(line));
	for(i=0;i<strlen(line);i++)
	{
		if(enable)
		{
			if(line[i]=='\t')line[i]=' ';
			if(!(space==1&&(line[i]==' ')))
				Line[k++]=line[i];
			if(line[i]=='"')
					enable=0;
			if(line[i]==' ')
				space=1;
			else
				space=0;
		}
		else
		{
			Line[k++]=line[i];
			if(line[i]=='"')
			{
				enable=1;
				space=0;
			}
		}
	}
	if(Line[k-1]==' ')Line[k-1]=0;
	Line[k]=0;
	return Line;
}
char **parse(char *superstring,char delimiter,int shrink)
{
	char **head;
	char *p=strtok(superstring,&delimiter);
	head=(char**)malloc(sizeof(char*)*N);
	int i=0;
	while (p!=NULL)
	{
			if(shrink)
				p=compress(p);
			if(*p)
			{
				*(head+i) = p;
				i++;
			}
			p=strtok (NULL,&delimiter);
	}
	*(head+i)=NULL;
	return head;
}
void clrptr(char **x)
{
  int i=0;
  while(*(x+i))
	{
		free(*(x+i));
		i++;
	}
  free(x);
}
void print(char **x)
{
  int i=0;
  while(*(x+i))
  {
    printf("%s\n",*(x+i));
    i++;
  }
}
