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
void changedir(char *directory)
{
  int ret=chdir(directory);
  if(ret)
    printf("vsh: cd: %s: No such file or directory\n",directory);
}
void echo(char *string)
{
  int i=0,state=0;
  for(i=0;i<strlen(string);i++)
  {
    if(string[i]!=' ')
      state++;
    if(state>=5)
      printf("%c",string[i]);
  }
  printf("\n");
}
void pwd()
{
  char *buf=getcwd(NULL,0);
  printf("%s\n",buf);
}
void ls(int show_hidden)
{
    int i=0,k=0;
    struct dirent **files;
    int n = scandir(".", &files, 0, alphasort);
      for(i=0;i<n;i++)
      {
        if(k%7==0&&k) printf("\n");
        if(!((files[i]->d_name)[0]=='.'&&show_hidden==0))
          {printf("%s   ",files[i]->d_name);k++;}
      }
      printf("\n");
}
void ls_long(int show_hidden)
{
  int i=0;
  char timer[14];
  struct dirent **files;
  struct stat details;
  int n = scandir(".",&files,0,alphasort);
  for(i=0;i<n;i++)
  {
    if(!((files[i]->d_name)[0]=='.'&&show_hidden==0))
      if(stat(files[i]->d_name,&details)==0)
      {
        printf("%1s",(S_ISDIR(details.st_mode)) ? "d" : "-");
        printf("%1s",(details.st_mode & S_IRUSR) ? "r" : "-");
        printf("%1s",(details.st_mode & S_IWUSR) ? "w" : "-");
        printf("%1s",(details.st_mode & S_IXUSR) ? "x" : "-");
        printf("%1s",(details.st_mode & S_IRGRP) ? "r" : "-");
        printf("%1s",(details.st_mode & S_IWGRP) ? "w" : "-");
        printf("%1s",(details.st_mode & S_IXGRP) ? "x" : "-");
        printf("%1s",(details.st_mode & S_IROTH) ? "r" : "-");
        printf("%1s",(details.st_mode & S_IWOTH) ? "w" : "-");
        printf("%1s ",(details.st_mode & S_IXOTH) ? "x" : "-");
        printf("%2ld ",(unsigned long)(details.st_nlink));
        printf("%10s ",(getpwuid(details.st_uid))->pw_name);
        printf("%10s ",(getgrgid(details.st_gid))->gr_name);
        printf("%10lld ",(unsigned long long)details.st_size);
        strftime (timer,14,"%h %d %H:%M",localtime(&details.st_mtime));
        printf("%s ",timer);
        printf("%s\n",files[i]->d_name);
      }
  }
}
int pinfo(int pid)
{
  if(pid==-1)
  pid=getpid();
  char hostname[N],Pstatus[N],buf[N],buff[N];
  int n=sprintf(buf,"/proc/%d/exe",pid);
  char *b1=buf,*b2=buff;
  ssize_t len=readlink(b1,b2,sizeof(buff));buff[len]=0;
  if(len==-1){printf("Process does not exist\n");return 0;}
  printf("pid -- %d\n",pid);
  n=sprintf(buf,"/proc/%d/status",pid);
  int hostname_file=open(buf,O_RDONLY);
  read(hostname_file,hostname,N*sizeof(char)+1);
  int k=0,I=0;
  for(n=0;n<100;n++)
  {
    if(hostname[n]==10){k++;continue;}
    if(k==2)
      Pstatus[I++]=hostname[n];
  }
  char *P=Pstatus;
  P=compress(P);
  char **words=parse(Pstatus,' ',1);
  printf("Process Status -- {%s}\n",words[1]);
  clrptr(words);
  close(hostname_file);
  n=sprintf(buf,"/proc/%d/statm",pid);
  hostname_file=open(buf,O_RDONLY);
  read(hostname_file,hostname,N*sizeof(char)+1);
  printf("- ");
  for(n=0;n<20;n++)
  {
    if(hostname[n]==32){k++;break;}
    printf("%c",hostname[n]);
  }
  printf(" {Virtual Memory}\n");
  printf("%s\n",buff);

}
void LSmap(char **argument)
{
  int j=0,a=0,l=0;
  while(*(argument+j)!=NULL)
  {
    if(strcmp(*(argument+j),"-a")==0)
    {a=1;}
    else if(strcmp(*(argument+j),"-l")==0)
    {l=1;}
    else if(strcmp(*(argument+j),"-al")==0)
    {a=1;l=1;}
    else if(strcmp(*(argument+j),"-la")==0)
    {a=1;l=1;}
    j++;
  }
  if(a&&l)ls_long(1);
  if(a&&!l)ls(1);
  if(l&&!a)ls_long(0);
  if(!l&&!a)ls(0);
}
void remindme(char *message,int waittime)
{
  int pid=fork();
  if(!pid)
  {
    sleep(waittime);
    printf("\n");
    int i=0,state=0;
    for(i=0;i<strlen(message);i++)
    {
      if(message[i]==' ')
        state++;
      if(state>=2)
        printf("%c",message[i]);   
    }
    printf("\n");
    exit(0);
  }
}
void redirect(char **pieces)
{
  int n=0,i,fd,fd2,sv0,sv1,fin=1,fout=0;
  sv0=dup(0);sv1=dup(1);
  char *file,redout1[3]=">",redout2[3]=">>",redin[2]="<";
  while(*(pieces+n)!=NULL)n++;
  for(i=n-1;i>=0;i--)
    if(strcmp(*(pieces+i),redout1)==0||strcmp(*(pieces+i),redout2)==0)
      break;
  if(i>0&&i!=n-1)
  {
    if(strcmp(*(pieces+i),redout1)==0)
      fd=open(*(pieces+i+1), O_WRONLY | O_TRUNC | O_CREAT, 0644);
    else if(strcmp(*(pieces+i),redout2)==0)
      fd=open(*(pieces+i+1), O_WRONLY | O_APPEND | O_CREAT, 0644);
    fout=1;
  }
  for(i=n-1;i>=0;i--)
    if(strcmp(*(pieces+i),redin)==0)
      break;
  if(i!=0&&i!=n-1)
  {
    fd2=open(*(pieces+i+1), O_RDONLY);
    if(fd2!=-1)
    {
      fin=1;
    }
  }
  for(i=0;i<n;i++)
  {
    if(strcmp(*(pieces+i),redout1)==0||strcmp(*(pieces+i),redout2)==0||strcmp(*(pieces+i),redin)==0)
      break;
  }
  for(;i<n;i++)
    *(pieces+i)=NULL;
  if(fin){dup2(fd2,0);
      close(fd2);}   
    if(fout){
      dup2(fd,1);
      close(fd);}
  int pid=fork();
  if(!pid)
  {
    execvp(*pieces,pieces);
  }
  else
  {
    wait(NULL);
  }
  dup2(sv0,0);
  dup2(sv1,1);
}
void Pipe(char **pieces)
{
  int pid,ppipe[2],noC,Stdin=0,sv0=dup(0),sv1=dup(1),i,j,n=0,c;
  char ***comm=(char ***)malloc(sizeof(char **)*1000);c=1;
  while(*(pieces+n)!=NULL)n++;
  noC=0;
  *(comm)=pieces;
  for(i=0;i<n;i++)
    if(strcmp(*(pieces+i),"|")==0)
    {
      noC++;
      *(pieces+i)=NULL;
      *(comm+c)=pieces+i+1;
      c++;
    }
  for(i=0;i<=noC;i++)
  {
    pipe(ppipe);
    pid=fork();
    if(!pid)
    {
      dup2(Stdin,0);
      if(dup2(ppipe[1],1)==1)
      {
        close(ppipe[0]);
        execvp(*(*(comm+i)),*(comm+i));        
        // exit(0);
      }
      else {printf("error\n");return;}
    }
    if(pid)
    {
      wait(NULL);
      close(ppipe[1]);
      Stdin=ppipe[0];
    }
  }
  dup2(sv0,0);
  dup2(sv1,1);

}
void FG(int pid)
{
  kill(pid,SIGCONT);
}