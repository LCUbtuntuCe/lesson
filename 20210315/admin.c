#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<sys/select.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<unistd.h>
#include<semaphore.h>

int main()
{
	int i,rfd,wfd,len=0,fd_in;
	char str[200];
	int flag,stdinflag;
	fd_set write_fd,read_fd;
	struct timeval net_timer;
        
        mkfifo("./fifo1",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
        
	wfd=open("fifo1",O_WRONLY);
        if(wfd<0)
	{
		printf("this pipe was failed to be opened!!\n");
		exit(-1);
	}
	printf("the process of Procon was successfully be set up:\n");

	while(1)
	{
		FD_ZERO(&read_fd);
		FD_SET(rfd,&read_fd);
		FD_SET(fileno(stdin),&read_fd);

		net_timer.tv_sec=5;
		net_timer.tv_usec=0;

		memset(str,0,sizeof(str));
		if(FD_ISSET(fileno(stdin),&read_fd))
		{
			printf("------------------------- the command is send by admin.c -------------------------\n");
			fgets(str,sizeof(str),stdin);
			len=write(wfd,str,strlen(str));
		}
		if(strstr(str,"exit")!=NULL)
		{
			printf("I decide to leave!!\n");
			exit(0);
		}
	}
	close(wfd);
}
