#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
void UnameHname()
{
	char *username,*hostname;
	hostname=(char*)malloc(10*sizeof(char));
	username=(char*)malloc(10*sizeof(char));
	username=getlogin();
	// x=getlogin();
	printf("%s\n",username);
	int hostname_file=open("/etc/hostname",O_RDONLY);
	read(hostname_file,hostname,10*sizeof(char)+1);
	int i;
	for(i=0;i<10*sizeof(char);i++)
	{
		if(hostname[i]==10)
		{
			hostname[i]=0;
			break;
		}
		// x
	}
	printf("%s\n",hostname);
}
int main()
{
	char *prompt=(char*)malloc(40*sizeof(char));
	UnameHname();
	return 0;
}
