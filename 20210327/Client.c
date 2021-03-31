#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/time.h>
#include<fcntl.h>

#define SERVPORT 4444
#define MAXSIZE 300
#define BACKLOG 10
#define STDIN 0

int main()
{
	int sockfd;
	int recvbytes;
	char buf[MAXSIZE];
	char *str;

	char name[MAXSIZE];
	char send_str[MAXSIZE];
	struct sockaddr_in serv_addr;
	fd_set wfd_set,rfd_set,efd_set;
        struct timeval timeout;
	memset(send_str,'\0',MAXSIZE);

	int ret;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("\nsocket error!\n");
		exit(1);
	}

	bzero(&serv_addr,sizeof(struct sockaddr_in));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERVPORT);
        inet_aton("192.168.136.1",&serv_addr.sin_addr);

	if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))==-1)
        {
	   perror("\nconnect error!\n");
	   exit(1);
	}

        fcntl(sockfd,F_SETFD,O_NONBLOCK);

	printf("\nI start to find server......\n");
        sprintf(send_str,"I am %lu.\n",pthread_self());
	//fflush(stdout);
	send(sockfd,send_str,strlen(send_str),0);
	memset(send_str,'\0',MAXSIZE);
	sleep(3);
	
        sprintf(send_str,"%lu Client: I want to connect with you.\n\n",pthread_self());
        send(sockfd,send_str,strlen(send_str),0);
	memset(send_str,'\0',MAXSIZE);

	while(1)
	{
	   FD_ZERO(&rfd_set);
           FD_ZERO(&wfd_set);
	   FD_ZERO(&efd_set);

	   //FD_SET(STDIN,&rfd_set);
	   FD_SET(sockfd,&rfd_set);
	   FD_SET(sockfd,&efd_set);

	   timeout.tv_sec=10;
	   timeout.tv_usec=0;

	   ret=select(sockfd+1,&rfd_set,&wfd_set,&efd_set,&timeout);
	   if(ret<0)
	   {
             perror("\nselect error!\n");
	     exit(1);
	   }

           if(strlen(send_str)!=0)
	   {
		   if(strstr(send_str,"quit")!=NULL)
		   {
			   close(sockfd);
			   exit(0);
		   }
		   send(sockfd,send_str,strlen(send_str),0);
                   memset(send_str,'\0',MAXSIZE);
	   }
	   
	   if(FD_ISSET(sockfd,&rfd_set))
	   {
              recvbytes=recv(sockfd,buf,MAXSIZE,0);
	      if(recvbytes==0)continue;
	      printf("%s",buf);
	      memset(buf,'\0',MAXSIZE);
	      printf("\nClient %lu:",pthread_self());
	      char s[225];
	      scanf("%s",s);
              sprintf(send_str,"%lu Client: %s\n",pthread_self(),s);
	   }
           if(FD_ISSET(sockfd,&efd_set))
	   {
	      close(sockfd);
	      exit(0);
	   }
	}
}
