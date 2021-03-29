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
#define STDIN 0

int main()
{
        int sockfd;
        int recvbytes;
        char buf[MAXDATASIZE];
        char name[MAXDATASIZE];
        char send_str[MAXDATASIZE];
        struct sockaddr_in serv_addr;

        fd_set rfd_set,wfd_set,efd_set;
        struct timeval timeout;
        int ret;

        sockfd=socket(AF_INET,SOCK_DGRAM,0);

        bzero(&serv_addr,sizeof(struct sockaddr_in));
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_port=htons(SERVPORT);
	serv_addr.sin_addr.s_addr=inet_addr("192.168.136.1");
        

	fcntl(sockfd,F_SETFD,O_NONBLOCK);
        printf("\nthe first thing is input your name:\n");
        scanf("%s",name);
        name[strlen(name)]='\0';
        fflush(stdout);

        sendto(sockfd,name,strlen(name),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

        sendto(sockfd,"I am LiuChang,I want to connect",strlen("I am LiuChang,I want to connect"),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
        
        while(1)
	{
                FD_ZERO(&rfd_set);
                FD_ZERO(&wfd_set);
                FD_ZERO(&efd_set);

                FD_SET(STDIN,&wfd_set);
                FD_SET(sockfd,&wfd_set);

                timeout.tv_sec=5;
                timeout.tv_usec=0;

                ret=select(0,NULL,NULL,NULL,&timeout);

                if(ret<0)
                {
                    perror("\nselect error!\n");
                    exit(-1);
                }
                if(FD_ISSET(STDIN,&wfd_set))
                {
                    //send_str[strlen(send_str)-1]='\0';
                    if(strstr(send_str,"quit")!=NULL)
                    {
			    sendto(sockfd,"I stop work",strlen("I stop work"),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
                            close(sockfd);
                            exit(0);
                    }
		    //send_str[strlen(send_str)-1]='\0';
                    sendto(sockfd,send_str,strlen(send_str),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
		    memset(send_str,'\0',MAXDATASIZE);
                }
                if(FD_ISSET(sockfd,&wfd_set))
                {
		   int len=sizeof(serv_addr);
		   recvbytes=recvfrom(sockfd,buf,MAXDATASIZE,0,(struct sockaddr*)&serv_addr,&len);

		   if(recvbytes<0)
		   {
                      perror("\nrecvfrom error!\n");
		      exit(1);
		   }

	           printf("\nServer:  %s\n",buf);
		   memset(buf,'\0',MAXDATASIZE);
		   printf("\n%s: ",name);
		   scanf("%s",send_str);
                }
	}
}
