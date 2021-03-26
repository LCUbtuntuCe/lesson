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

#define STDIN 0

int main()
{
	struct timeval tv;
	char str[50];
	fd_set readfds;
	tv.tv_sec=10;
	tv.tv_usec=500000;
	FD_ZERO(&readfds);
	FD_SET(STDIN,&readfds);
        printf("What's your name?\n");
	select(STDIN+1,&readfds,NULL,NULL,&tv);
	if(FD_ISSET(STDIN,&readfds))
	{
		read(STDIN,str,50);
		printf("Your name is %s\n",str);
	}
	else
		printf("You have be later........\n");
}
