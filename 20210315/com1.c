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


int Judge(int rfd,int wfd)
{
	if(rfd<0||wfd<0)
	{
		printf("the pipe cannot be used!!\n");
		return -1;
	}
	else return 0;
}

int main()
{
	int i,rfd,wfd,len=0,fd_in;
	char str[20],str1[20];
	int flag,stdinflag;
	fd_set write_fd,read_fd;
	struct timeval net_timer;
        
	mkfifo("./fifo1",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	mkfifo("./fifo2",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
        
	if(wfd=open("./fifo1",O_WRONLY|O_NONBLOCK)<0||errno==ENXIO)
        {
	   rfd=open("./fifo1",O_RDONLY|O_NONBLOCK);
	   wfd=open("./fifo2",O_WRONLY);
	}
	else
	{
	   rfd=open("./fifo2",O_RDONLY|O_NONBLOCK);
	   wfd=open("./fifo1",O_WRONLY);
	} 

	if(Judge(rfd,wfd)==-1)exit(-1);
	printf("it's the screen of process%d:\n",getpid());

	while(1)
	{
		FD_ZERO(&read_fd);
		FD_SET(rfd,&read_fd);
		FD_SET(fileno(stdin),&read_fd);

		net_timer.tv_sec=5;
		net_timer.tv_usec=0;

		memset(str,0,sizeof(str));
		memset(str1,0,sizeof(str1));
		if(i=select(rfd+1,&read_fd,NULL,NULL,&net_timer)<=0)continue;
		if(FD_ISSET(rfd,&read_fd))
		{
			read(rfd,str,sizeof(str));
			printf("-----------------------------------------\n");
            printf("%s\n",str);
		}
		if(FD_ISSET(fileno(stdin),&read_fd))
		{
			printf("-----------------------------------------\n");
			char ch[11]="you friend:";
			fgets(str1,sizeof(str1),stdin);
			sprintf(str,"%s%d\n",ch,getpid());
			strcat(str,str1);
			len=write(wfd,str,strlen(str));
		}
		if(strstr(str,"exit")!=NULL)
		{
			printf("I decide to leave!!\n");
			exit(0);
		}
	}
	close(rfd);
	close(wfd);
}
