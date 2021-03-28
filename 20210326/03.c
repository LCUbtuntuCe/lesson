#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<errno.h>
#include<sys/types.h>
#include <unistd.h>
#include<sys/time.h>
#include<fcntl.h>

#define MAXDATASIZE 256
#define SERVPORT 10011
#define STDIN 0

int main()
{
	int sockfd;
	int recvbytes;
	char buf[MAXDATASIZE];
	char *str;

	char name[MAXDATASIZE];
	char send_str[MAXDATASIZE];
	struct sockaddr_in serv_addr;
	int len=sizeof(serv_addr);

	fd_set rfd_set,wfd_set,efd_set;
	struct timeval timeout;
	int ret;
	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		perror("socket error!\n");
		exit(1);
	}

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERVPORT);
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	fcntl(sockfd,F_SETFD,O_NONBLOCK);
	bzero(&(serv_addr.sin_zero),8);
	printf("the first thing is input your name:\n");

	scanf("%s",name);
	name[strlen(name)]='\0';
	printf("\n%s: \n",name);
	fflush(stdout);
	sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&serv_addr,len);

	while(1)
	{
		FD_ZERO(&rfd_set);
		FD_ZERO(&wfd_set);
		FD_ZERO(&efd_set);

		FD_SET(STDIN,&rfd_set);
		FD_SET(sockfd,&rfd_set);
		FD_SET(sockfd,&efd_set);
		timeout.tv_sec=10;
		timeout.tv_usec=0;

		ret=select(sockfd+1,&rfd_set,&wfd_set,&efd_set,&timeout);
		if(ret==0)
		{
			continue;
		}
		else if(ret<0)
		{
                    perror("select error!\n");
		    exit(-1);
		}
		if(FD_ISSET(STDIN,&rfd_set))
		{
		    fgets(send_str,256,stdin);
		    send_str[strlen(send_str)-1]='\0';
		    if(strncmp("quit",send_str,4)==0)
		    {
			    close(sockfd);
			    exit(0);
		    }
		    sendto(sockfd,send_str,strlen(send_str),0,(struct sockaddr*)&serv_addr,len);
		}
		if(FD_ISSET(sockfd,&rfd_set))
		{
			recvbytes=recvfrom(sockfd,buf,MAXDATASIZE,0,(struct sockaddr*)&serv_addr,&len);
			if(recvbytes==0)
			{
				close(sockfd);
				exit(0);
			}
			buf[recvbytes]='\0';
			printf("Server: %s\n",buf);

			printf("%s:\n",name);
			fflush(stdout);
		}
                if(FD_ISSET(sockfd,&efd_set))
		{
		   close(sockfd);
		   exit(0);
		}                                                                                                                                            
	}
}
