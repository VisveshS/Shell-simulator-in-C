#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include<dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#include<limits.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include "Functions.h"
#define N 1001
#define M 200
void cases(char *command)
{
  char *command_copy=(char*)malloc(1001);
  strcpy(command_copy,command);
  char **pieces=parse(command,' ',1);
  int i=0,redir=0,ppipe=0;
  i=0;
  while(*(pieces+i)!=NULL)
  {
    if(strcmp(*(pieces+i),"<")==0)redir=1;
    else if(strcmp(*(pieces+i),">")==0)redir=1;
    else if(strcmp(*(pieces+i),">>")==0)redir=1;
    if(strcmp(*(pieces+i),"|")==0)ppipe=1;
    i++;
  }
  int fgbg,j=0;
  while(*(pieces+j)!=NULL)j++;
  if(strcmp(*(pieces+j-1),"&")==0)
  {
      *(pieces+j-1)=NULL;
      pid_t pid=fork();
      if(!pid)
      {
        execvp(*pieces,pieces);
        exit(0);
      }
      printf("[+] %d\n",pid);
      addBGP(pid);
  }
  else if(redir&&!ppipe)
  {
    redirect(pieces);
  }
  else if(ppipe)
  {
    Pipe(pieces); 
  }
  else if(strcmp(*pieces,"cd")==0)
  {
    if(i==2)
      changedir(*(pieces+1));
    else
      printf("vsh: cd: too many arguments\n");
  }
  else if(strcmp(*pieces,"bg")==0)
  {
    int pid;
    printf("BG\n");
    if(*(pieces+1)!=NULL)
    {
    pid = atoi(*(pieces+1));
    printf("%d\n",pid);
    kill(pid,25);}
    else {fprintf(stderr, "not enough arguments !\n");}
  }
  else if(strcmp(*pieces,"fg")==0)
  {
    int pid;
    printf("FG\n");
    if(*(pieces+1)!=NULL)
    {
    pid = atoi(*(pieces+1));
    printf("%d\n",pid);
    FG(pid);}
    else
      {fprintf(stderr, "not enough arguments !\n");}
  }
  else if(strcmp(*pieces,"jobs")==0)
  {
    printBG();
  }
  else if(strcmp(*pieces,"echo")==0)
  {
    echo(command_copy);
  }
  else if(strcmp(*(pieces),"pwd")==0)
    pwd();
  else if(strcmp(*(pieces),"ls")==0)
  {
    LSmap(pieces);
  }
  else if(strcmp(*(pieces),"pinfo")==0)
  {
    if(*(pieces+1)==NULL)pinfo(-1);
    else
    {
      int pid=0,i;
      for(i=0;i<strlen(*(pieces+1));i++)
      {
        pid*=10;
        pid+=(*(pieces+1))[i]-'0';
      }
      pinfo(pid);
    }
  }
  else if(strcmp(*(pieces),"overkill")==0)
  {
    overkill();
  }
  else if(strcmp(*(pieces),"kjob")==0)
  {
    int jobno,signo;
    if(*(pieces+1)!=NULL)
      if(*(pieces+2)!=NULL)
      {
        jobno=atoi(*(pieces+1));
        signo=atoi(*(pieces+2));
        kjob(jobno,signo);
      }
  }
  else if(strcmp(*(pieces),"remindme")==0)
  {
    int i,waittime=0;
    for(i=0;i<strlen(*(pieces+1));i++)
    {
      waittime*=10;
      waittime+=(*(pieces+1))[i]-'0';
    }
    remindme(command_copy,waittime);
  }
  else if(strcmp(*(pieces),"exit")==0)
    exit(0);
  else
  {
    pid_t pid=fork();
    if(!pid)
    {
      execvp(*pieces,pieces);
      exit(0);
    }
    else
      wait(NULL);
  free(command_copy); 
  }
}
