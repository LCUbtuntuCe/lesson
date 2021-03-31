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

#define MAXSIZE 300
#define SERVPORT 4444
#define BACKLOG 10
#define STDIN 0
#define MaxNumberP 8

int sockfd;
pthread_t tid[8];
long Num=0;//the number of how many pthreads my program has created
fd_set rfd_set,wfd_set,efd_set;
struct sockaddr_in my_addr,remote_addr;

//these datas is needed to be used by several pthreads meanwhile:
char *client_ph[MaxNumberP];
int client_fd;
sem_t costumer[MaxNumberP];
char send_str[MAXSIZE];//this string can only send to one client
sem_t send_inform;

void Sem_prepare()
{
	for(int i=0;i<MaxNumberP;i++)
	{
		int res=sem_init(&costumer[i],0,0);
		if(res!=0)
		{
			perror("\nSemaphore init failed!\n");
			exit(1);
		}
	}
}

void Server_func(int i)
{
    while(1)
    {
        sem_wait(&costumer[i]);
        char s[200];
        if(FD_ISSET(STDIN,&rfd_set))
        {
          printf("\nThe %dth Serve for %s:\n",i,client_ph[i]);
          scanf("%s",s);
          sem_wait(&send_inform);
          sprintf(send_str,"\nServer %lu: %s\n",pthread_self(),s);
          send(client_fd,send_str,strlen(send_str),0);
          sem_post(&send_inform);
        }
    }
}

void Connect()
{
	int sin_size;
	sin_size=sizeof(struct sockaddr_in);
	if((client_fd=accept(sockfd,(struct sockaddr*)&remote_addr,&sin_size))==-1)
        {
	   perror("\naccept error!\n");
	   exit(1);
	}
	//printf("\nSuccessfully connected!\n");
	fcntl(client_fd,F_SETFD,O_NONBLOCK);
}

int main()
{
    Sem_prepare();
    sem_init(&send_inform,0,1);
    for(int i=0;i<MaxNumberP;i++)client_ph[i]=NULL;

    //int sin_size;
    char buff[MAXSIZE];
    int recvbytes;
    
    struct timeval timeout;
    int ret;
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
	    perror("\nsocket error!\n");
	    exit(1);
    }

    bzero(&my_addr,sizeof(struct sockaddr_in));
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(SERVPORT);
    inet_aton("192.168.136.1",&my_addr.sin_addr);

    if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))==-1)
    {
	    perror("\nbind error!\n");
	    exit(1);
    }

    if(listen(sockfd,BACKLOG)==-1)
    {
	    perror("\nlisten error\n");
	    exit(1);
    }
    //sin_size=sizeof(struct sockaddr_in);
    /*if((client_fd=accept(sockfd,(struct sockaddr*)&remote_addr,&sin_size))==-1)
    {
	    perror("\naccept error!\n");
	    exit(1);
    }
    printf("\nSuccessfully connected!\n");

    fcntl(client_fd,F_SETFD,O_NONBLOCK);*/
    
    while(1)
    {
	    Connect();
	    FD_ZERO(&rfd_set);
            FD_ZERO(&wfd_set);
	    FD_ZERO(&efd_set);
            
	    FD_SET(STDIN,&rfd_set);
	    FD_SET(client_fd,&rfd_set);
	    FD_SET(client_fd,&wfd_set);
	    FD_SET(client_fd,&efd_set);

	    timeout.tv_sec=10;
	    timeout.tv_usec=0;

	    ret=select(client_fd+1,&rfd_set,&wfd_set,&efd_set,&timeout);
	    if(ret==0)
	    {
		    continue;
	    }
	    else if(ret<0)
	    {
		    perror("\nselect error!\n");
		    exit(1);
	    }
            
	    if(FD_ISSET(client_fd,&rfd_set))
            {
	       recvbytes=recv(client_fd,buff,MAXSIZE,0);
	       if(strncmp(buff,"I am ",5)==0)
	       {
		  Num++;
                  client_ph[Num-1]=(char*)malloc(sizeof(char)*17);
		  memset(client_ph[Num-1],'\0',17);
                  for(int i=5;i<=20;i++)client_ph[Num-1][i-5]=buff[i];
                  if((pthread_create(&tid[Num-1],NULL,(void*)Server_func,(void*)(Num-1)))!=0)
		  {
                     perror("\nfailed to create new pthread!\n");
		     exit(1);
		  }
		  memset(buff,'\0',MAXSIZE);
	          send(client_fd,"Server: We have arranged serve for you!\n",sizeof("Server: We have arranged serve for you!\n"),0);
	       }
	       else 
               {
	          for(int i=0;i<Num;i++)
		  {
		     if(strncmp(buff,client_ph[i],15)==0)
		     {
			     printf("\n%s\n",buff);
			     sem_post(&costumer[i]);
		     }
		  }
               }
	       memset(buff,'\0',MAXSIZE);
            }
    }
    for(int i=0;i<Num;i++)pthread_join(tid[i],NULL);
}
