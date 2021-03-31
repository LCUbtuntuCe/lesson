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

#define COUT 5

int socket_fd[COUT];

void pthread_function(int client_fd)
{
	char message[1500];
	char buf[1024];
	int i,recvbytes;
	char name[20];

	recvbytes=recv(client_fd,name,20,0);

	name[recvbytes]=':';
        name[recvbytes+1]='\0';
	while(1)
	{
		if((recvbytes=recv(client_fd,buf,1024,0))==-1)
		{
			perror("\nrecv error!\n");
			exit(1);
		}
		if(recvbytes==0)
	        {
		   printf("\n%s bye!\n",name);
		   break;
		}
		else
		{
                   printf("\n%s %s\n",name,buf);
		}
		buf[recvbytes]='\0';
                for(i=0;i<COUT;i++)
		{
	           if(socket_fd[i]==-1){
		     continue;
		   }
		   else
		   {
                      message[0]='\0';
		      char buf1[20];
		      printf("\nServe: \n");
		      scanf("%s",buf1);
                      strcat(message,name);
		      strcat(message,buf1);
		      if(send(socket_fd[i],message,strlen(message),0)==-1)
		      {
			      perror("\nsend error!\n");
			      exit(1);
		      }
		   }
		}
          }
	   close(client_fd);
           for(i=0;i<COUT;i++)
	   {
	      if(socket_fd[i]==client_fd)
	      {
                    socket_fd[i]=-1;
              }
	   }

	   pthread_exit(NULL);
}

int main()
{
   int i;
   for(i=0;i<COUT;i++)socket_fd[i]=-1;

   pthread_t id;
   long sockfd,client_fd;
   socklen_t sin_size;

   struct sockaddr_in my_addr,remote_addr;

   if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
   {
      perror("\nsocket error!\n");
      exit(1);
   }

    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(12345);
    my_addr.sin_addr.s_addr=INADDR_ANY;
    bzero(&(my_addr.sin_zero),8);

    if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))==-1)
    {
       perror("\nbind error!\n");
       exit(1);
    }

    if(listen(sockfd,10)==-1)
    {
       perror("\nlisten error\n");
       exit(1);
    }

    i=0;
    while(1)
    {
	    sin_size=sizeof(struct sockaddr_in);
	    if((client_fd=accept(sockfd,(struct sockaddr*)&remote_addr,&sin_size))==-1)
            {
	       perror("\naccept error!\n");
	       exit(1);
	    }
	    while(socket_fd[i]!=-1)i=(i+1)%COUT;
	    socket_fd[i]=client_fd;
	    pthread_create(&id,NULL,(void*)pthread_function,(int *)(client_fd));
    }
}
