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

char recv_buf[1500],send_buf[1024];
char name[20];

void pthread_function(int sockfd)
{
	int recvbytes;
	while(1)
	{
		if((recvbytes=recv(sockfd,recv_buf,1500,0))==-1)
                {
		  perror("\nrecv error!\n");
		  exit(1);
		}
		else
		{
			if(strncmp(recv_buf,name,5)==0)
			{
			   recv_buf[recvbytes]='\0';
			   printf("Serve \n%s\n",&recv_buf[6]);
			}
		}
	}
}

int main()
{
	pthread_t id;
	long sockfd;
	struct sockaddr_in server_addr;

	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(12345);
        server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
	   perror("\nsocket error!\n");
	   exit(1);
	}

	if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
        {
	   perror("\nconnect error!\n");
	   exit(1);
	}
 
	printf("\nInput your name:\n");
	scanf("%s",name);
	send(sockfd,name,strlen(name),0);
        pthread_create(&id,NULL,(void*)pthread_function,(int *)(sockfd));

	while(1)
	{
		printf("\n%s:\n",name);
		scanf("%s",send_buf);
		if(send(sockfd,send_buf,strlen(send_buf),0)==-1)
	        {
		   perror("\nsend error!\n");
		   exit(1);
		}
		sleep(1);
	}

	close(sockfd);
	pthread_cancel(id);
	return 0;
}
