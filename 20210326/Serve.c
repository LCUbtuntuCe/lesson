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
#define SERVPORT 4444
#define BACKLOG 10
#define STDIN 0

int main()
{
        int sockfd,client_fd;
        int sin_size;
        struct sockaddr_in my_addr,remote_addr;
        char buf1[256];
        char buf2[256];
        char send_str[256];
        int recvbytes;
        fd_set rfd_set,wfd_set,efd_set;
        struct timeval timeout;
        int ret;

        sockfd=socket(AF_INET,SOCK_DGRAM,0);

        bzero(&my_addr,sizeof(struct sockaddr_in));
        my_addr.sin_family=AF_INET;
        my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr=inet_addr("192.168.136.1");
        //inet_aton("127.0.0.1",&my_addr.sin_addr);

        if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))==-1)
        {
                perror("bind error!\n");
                exit(1);
        }

        /*if(listen(sockfd,BACKLOG)==-1)
        {
                perror("listen error!\n");
                exit(1);
        }*/

        sin_size=sizeof(struct sockaddr_in);
        /*if((client_fd=accept(sockfd,(struct sockaddr*)&remote_addr,&sin_size);)==-1)*/
        /*{
                perror("accept error!\n");
                exit(1);
        }*/
        //fcntl(client_fd,F_SETFD,O_NONBLOCK);
        /*int len=sizeof(remote_addr);
        recvbytes=recvfrom(client_fd,buf2,MAXDATASIZE,0,(struct sockaddr*)&remote_addr,&len);
        buf2[recvbytes]='\0';*/

        fflush(stdout);

        //fprintf(Fp,"%s\n",buf2);
        while(1)
        {
                FD_ZERO(&rfd_set);
                FD_ZERO(&wfd_set);
                FD_ZERO(&efd_set);
				
                FD_SET(STDIN,&rfd_set);
                FD_SET(sockfd,&rfd_set);
                FD_SET(sockfd,&wfd_set);
                FD_SET(sockfd,&efd_set);

                timeout.tv_sec=10;
                timeout.tv_usec=0;
                
                ret=select(sockfd+1,&rfd_set,&wfd_set,&efd_set,&timeout);

                if(ret<0)
                {
			perror("select error!\n");
			exit(-1);
                        //continue;
                }
		if(ret==0)continue;

                /*if(ret<0)
                {
                        perror("select error!\n");
                        exit(-1);
                }*/
                if(FD_ISSET(STDIN,&wfd_set))
                {
                        fgets(send_str,256,stdin);
                        send_str[strlen(send_str)-1]='\0';
                        if(strncmp("quit",send_str,4)==0)
                        {
                                //close(client_fd);
                                close(sockfd);
                                exit(0);
                        } 
                        sendto(sockfd,send_str,strlen(send_str),0,(struct sockaddr*)&remote_addr,sizeof(remote_addr));
                }
                if(FD_ISSET(sockfd,&rfd_set))
                {
		   int len2=sizeof(remote_addr);
                   recvbytes=recvfrom(sockfd,buf1,MAXDATASIZE,0,(struct sockaddr*)&remote_addr,&len2);
                   if(recvbytes==0)
                   {
                           //close(client_fd);
                           close(sockfd);
                           exit(0);
                   }
                   buf1[recvbytes]='\0';
                   printf("%s:%s\n",buf2,buf1);
                   printf("Server:\n");
                   fflush(stdout);
                }
                if(FD_ISSET(sockfd,&efd_set))
                {
                        close(sockfd);
                        exit(0);
                }
        }
}
