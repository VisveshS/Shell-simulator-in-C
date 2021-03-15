#include<dirent.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>
#include<stdlib.h>
#include<limits.h>
#include<sys/utsname.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
void addtoarray(int x,int p)
{
	static int arr[10],siz=0,i,n;
	if(!p){
	arr[siz++]=x;}
	else
	{for(i=0;i<n;i++)printf("%d ",arr[i]);printf("\n");}
}
int main()
{
	int n,i,x;
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{scanf("%d",&x);addtoarray(x,0);}
	addtoarray(0,1);
	return 0;
}
