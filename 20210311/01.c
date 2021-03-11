#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define PIPEFILE "./fifo1"
int main()
{
	int fd;
	char *msg="hhhhhhhhhh!\n";
	printf("before fifo open\n");
	if((fd=open(PIPEFILE,O_WRONLY))<0)
	{
		perror("fifo open failed!\n");
		exit(-1);
	}
	else
	{
		printf("fifo open failed!\n");
	}
	printf("after fifo open\n");
	write(fd,msg,10);
	close(fd);
	return 0;
}
