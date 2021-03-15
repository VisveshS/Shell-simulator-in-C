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
int *BGP;
int main()
{
	BGP=(int *)malloc(sizeof(int)*1000);
	storeBG();
	while(1)
	{
		if(getpid())
		prompt();
		char iparray[1001],*ipp,**head;ipp=iparray;
		ipp=input();
		ipp=compress(ipp);
		head=parse(ipp,';',1);
		int i=0;
		while(*(head+i))
		{
			cases(*(head+i));
			i++;
		}
		clrptr(head);
		Done();
	}
	return 0;
}
