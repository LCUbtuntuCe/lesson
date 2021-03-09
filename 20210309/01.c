#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc,const char* argv[])
{
	FILE *fp;
	int ch;
	if(argc<2)
	{
		printf("please input filename.\n");
		exit(0);
	}
	if(!strcmp("--help",argv[1]))
	{
		printf("this is a program stimulating Linux cat command.\n");
		printf("usage: ./mycat filename\n");
		exit(0);
	}
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("file \"%s\" open error!\n",argv[1]);
		exit(0);
	}
	while((ch=fgetc(fp))!=EOF)putchar(ch);
	fclose(fp);
	return 0;
}
