#include<stdlib.h>
#include<stdio.h>

int main()
{
	FILE* in,*out;
	char ch,infile[10],outfile[10];
	printf("enter the infile name:\n");
	scanf("%s",infile);
	printf("enter the outfile name:\n");
	scanf("%s",outfile);
	if((in=fopen(infile,"r"))==NULL)
	{
		printf("cannot open infile\n");
		exit(0);
	}
	if((out=fopen(outfile,"w"))==NULL)
	{
		printf("cannot open outfile\n");
		exit(0);
	}
	while(!feof(in))fputc(fgetc(in),out);
	fclose(in);
	fclose(out);
}
