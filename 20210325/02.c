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

int port=10086;

int main()
{
	int sockfd;
	int i=0;
	int z;
	char buf[80],str1[89];
        
	struct sockaddr_in adr_server;
	FILE* fp;
	printf("open the file..........\n");
	fp=fopen("liu","r");
	if(fp==NULL)
	{
		perror("open the file failed!\n");
		exit(1);
	}
	printf("connect the sever..........\n");

	adr_server.sin_family=AF_INET;
	adr_server.sin_port=htons(port);
	adr_server.sin_addr.s_addr=inet_addr("192.168.1.173");
	bzero(&(adr_server.sin_zero),8);
	sockfd=socket(AF_INET,SOCK_DGRAM,0);

	if(sockfd==-1)
	{
		perror("socket error!\n");
		exit(1);
	}
	printf("send the file................\n");
        while(1)
	{
		fgets(str1,80,stdin);
		printf("%d:%s\n",i,str1);
                sprintf(buf,"%d:%s",i,str1);
		z=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&adr_server,sizeof(adr_server));
		if(z<0)
		{
			perror("recvfrom error!\n");
			exit(1);
		}
		if(strstr(str1,"stop")!=NULL)
	        {
		   break;
	        }
	}

	printf("send.................\n");
	sprintf(buf,"stop\n");
	z=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&adr_server,sizeof(adr_server));
	if(z<0)
	{
		perror("sendto error!\n");
		exit(1);
	}
	fclose(fp);
	close(sockfd);
	exit(0);
}
