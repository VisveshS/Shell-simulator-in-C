#include<stdio.h>
#include<stdlib.h>
char *vsh_getline()
{
	char *x,c='a';int buffer_size=10,position=0;
	x=(char*)malloc(sizeof(char)*10);
	while(1)
	{
		c=getchar();
		if(c==10)break;
		x[position++]=c;
		if(buffer_size<=position)
		{
			buffer_size+=10;
			x=realloc(x,sizeof(char)*buffer_size);
		}
	}
	return x;
}
char **vsh_parse(char *x,char delimiter)
{
	char **y,Y[100][100];
	while(*x)
	{
		if(*x==delimiter)
		{
			no_of_words++;
			i=0;
			buffer_size=10;
			y[no_of_words]=(char*)malloc(sizeof(char)*buffer_size);
			if(no_of_words==sentence_block)
			{
				sentence_block+=5;
				y=realloc(y,sizeof(*y)*sentence_block);
			}
		}
		else
		{
			if(i>=buffer_size-1)
			{
				buffer_size+=10;
				y[no_of_words]=(char*)realloc(y[no_of_words],sizeof(char)*buffer_size);
			}
			y[no_of_words][i++]=*x;
		}
		x++;
	}
	return y;
}
void vsh_free(char **y)
{
	int i;char **X=y;
	while(*X)
	{
		free(*X);
		X++;
	}
}
int main()
{
	char *x,**y;
	while(1)
	{
		x=vsh_getline();
		y=vsh_parse(x,32);
		printf("<%s> becomes <%s> and <%s>\n",x,y[0],y[1]);
		vsh_free(y);
		free(x);
	}
	return 0;
}
