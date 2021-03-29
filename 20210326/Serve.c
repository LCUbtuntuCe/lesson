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

#define MAXDATASIZE 2048
#define SERVPORT 4444
#define BACKLOG 10
#define STDIN 0

int main()
{
        int sockfd;
        struct sockaddr_in my_addr,remote_addr;
        char buf2[2048];
        char send_str[256];
        int recvbytes;
        fd_set rfd_set,wfd_set,efd_set;
        struct timeval timeout;
        int ret;
	memset(&remote_addr,0,sizeof(remote_addr)); 

	bzero(&(my_addr.sin_zero),8);
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        my_addr.sin_addr.s_addr=inet_addr("192.168.136.1");

        sockfd=socket(AF_INET,SOCK_DGRAM,0);

        if((bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr)))==-1)
        {
                perror("\nbind my_addr error!\n");
                exit(1);
        }
	fcntl(sockfd,F_SETFD,O_NONBLOCK);

        int len=sizeof(remote_addr);
        recvbytes=recvfrom(sockfd,buf2,MAXDATASIZE,0,(struct sockaddr*)&remote_addr,&len);
        buf2[recvbytes]='\0';
	printf("\nSuccessfully connected with: %s\n",buf2);

        fflush(stdout);
        
	int p=0;
        while(1)
        {
                FD_ZERO(&rfd_set);
                FD_ZERO(&wfd_set);
                FD_ZERO(&efd_set);
				 
                FD_SET(sockfd,&wfd_set);

                timeout.tv_sec=5;
                timeout.tv_usec=0;

                ret=select(0, NULL,&wfd_set, NULL, &timeout);
                if(ret<0)
                {
			perror("\nselect error!\n");
			exit(-1);
		}

                if(p!=0)
                {
                        //send_str[strlen(send_str)-1]='\0';
                        if(strstr(send_str,"quit")!=NULL)
                        {
				sendto(sockfd,"I stop work",strlen("I stop work"),0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
				sleep(2);
                                close(sockfd);
                                exit(0);
                        }
			//send_str[strlen(send_str)-1]='\0';
                        sendto(sockfd,send_str,strlen(send_str),0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
			memset(send_str,'\0',strlen(send_str));
			p=0;
                }
                if(FD_ISSET(sockfd,&wfd_set))
                {
		   char buf1[2048];
		   int len2=sizeof(remote_addr);
		   sleep(2);
                   recvbytes=recvfrom(sockfd,buf1,MAXDATASIZE,0,(struct sockaddr*)&remote_addr,&len2);
		   while(recvbytes<=0)
		   {
			   recvbytes=recvfrom(sockfd,buf1,MAXDATASIZE,0,(struct sockaddr*)&remote_addr,&len2);
			   if(recvbytes<0)
		           {
			      printf("\nrecvfrom error!\n");
			      exit(1);
			   }
		   }
                   //buf1[recvbytes]='\0';
                   printf("\n%s:%s\n",buf2,buf1);
		   memset(buf1,'\0',MAXDATASIZE);
                   printf("\nServer:\n");
		   scanf("%s",send_str);
		   p=1;
                }
        }
}
