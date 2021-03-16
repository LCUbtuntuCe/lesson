#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/select.h>
#include<semaphore.h>

char str[100];
sem_t full,empty;

char** ptd=NULL;
int pd=0;
char** ctd=NULL;
int cd=0;

struct msg
{
	struct msg* next;
	int num;
};
struct msg* head;
pthread_cond_t has_product=PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

void enlargeP(char* q1)
{
     if(pd==0)
     {
	     ptd=(char**)malloc(sizeof(char*));
	     ptd[0]=(char*)malloc(sizeof(char)*200);
	     strcpy(ptd[0],q1);
     }
     else{
	     char **a;
	     a=(char**)malloc(sizeof(char*)*(pd+2));
	     for(int i=0;i<(pd+1);i++)a[i]=(char*)malloc(sizeof(char)*200);
	     for(int i=0;i<pd;i++)a[i]=ptd[i];
             a[pd]=q1;
	     free(ptd);
	     ptd=NULL;
	     ptd=a;
     }
     pd++;
     printf("there is %d producers in the computer.\n",pd);
}

void enlargeC(char* q2)
{
    if(cd==0)
    {
       ctd=(char**)malloc(sizeof(char*));
       ctd[0]=(char*)malloc(sizeof(char)*200);
       strcpy(ctd[0],q2);
    }
    else{
       char **b;
       b=(char**)malloc(sizeof(char*)*(cd+2));
       for(int i=0;i<(cd+1);i++)b[i]=(char*)malloc(sizeof(char)*200);
       for(int i=0;i<cd;i++)b[i]=ctd[i];
       b[cd]=q2;
       free(ctd);
       ctd=NULL;
       ctd=b;
     }
     cd++;
     printf("there is %d concumers in the computer.\n",cd);
}

void* producer(void* p)
{
	char q1[100];
	if(strstr(str,"create producer")!=NULL)printf("I am the producer: %lu\n",pthread_self());
        sprintf(q1,"%lu",pthread_self());
	enlargeP(q1);

	struct msg* mp;
	while(1)
	{
            sem_wait(&empty);
	    mp=malloc(sizeof(struct msg));
	    mp->num=rand()%1000+1;
	    pthread_mutex_lock(&lock);
	    mp->next=head;
	    head=mp;
	    pthread_mutex_unlock(&lock);
	    pthread_cond_signal(&has_product);
	    sem_post(&full);
	    sleep(rand()%5);
	    pthread_testcancel();
	}
}

void* consumer(void* p)
{
	char q2[100];
	if(strstr(str,"create consumer")!=NULL)printf("I am the consumer: %lu\n",pthread_self());
	sprintf(q2,"%lu",pthread_self());
	enlargeC(q2);
        
	struct msg* mp;
	while(1)  
	{
	   sem_wait(&full);
	   pthread_mutex_lock(&lock);
	   while(head==NULL)pthread_cond_wait(&has_product,&lock);
	   mp=head;
	   head=mp->next;
	   pthread_mutex_unlock(&lock);
	   free(mp);
	   sem_post(&empty);
	   sleep(rand()%5);
	   pthread_testcancel();
	}
}

void OutputP()
{
	printf("all of the ID of producer:\n");
	if(pd>0)for(int i=0;i<pd;i++)printf("%s \n",ptd[i]);
	else printf("there is no producer!\n");
}

void OutputC()
{
	printf("all of the ID of consumer:\n");
        if(cd>0)for(int i=0;i<cd;i++)printf("%s \n",ctd[i]);
	else printf("there is no consumer!\n");
}

int Delete(char num[])
{
    for(int i=0;i<pd;i++)
    {
	    if(strcmp(ptd[i],num)==0)
	    {
		    ptd[i]=ptd[pd-1];
		    pd--;
		    return 0;
	    }
    }
    for(int i=0;i<cd;i++)
    {
       if(strcmp(ctd[i],num)==0)
       {
	  ctd[i]=ctd[cd-1];
	  cd--;
	  return 0;
       }
    }
    return -1;

}

void deal_delete()
{
	unsigned long s=0;
	char num[50];
	for(int i=7;i<strlen(str)-1;i++)num[i-7]=str[i];
	for(int i=0;i<strlen(num);i++)
	{
		s*=10;
		s+=(num[i]-'0');
	}
        if(Delete(num)==-1)
	{
		printf("Failed\n");
		return;
	}
        if(pthread_cancel((pthread_t)s)==0)
	{
		pthread_join((pthread_t)s,NULL);
		printf("OK!\n");
	}
	else printf("Failed\n");
}

void end()
{
	pthread_mutex_destroy(&lock);
	sem_destroy(&full);
	sem_destroy(&empty);
}

int main(int argc,char* argv[])
{
    sem_init(&full,0,0);
    sem_init(&empty,0,5);
    fd_set read_fd;
    int rfd;
    struct timeval net_timer;
    pthread_t pt,ct;

    mkfifo("./fifo1",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
    printf("\nI am procon!\n");
    rfd=open("./fifo1",O_RDONLY|O_NONBLOCK);
    if(rfd<=0){
	    printf("I cannot read the command,so I forced to exit!\n");
	    exit(-1);
    }

    while(1)
    {
	 int i;
         FD_ZERO(&read_fd);
	 FD_SET(rfd,&read_fd);
	 FD_SET(fileno(stdin),&read_fd);

	 net_timer.tv_sec=5;
	 net_timer.tv_usec=0;
 
	 memset(str,0,sizeof(str));
	 if(i=select(rfd+1,&read_fd,NULL,NULL,&net_timer)<=0)continue;
	 printf("************************ the response from procon *****************************\n");
	 if(FD_ISSET(rfd,&read_fd))
         {
	    read(rfd,str,sizeof(str));
	    if(strstr(str,"create producer")!=NULL)pthread_create(&pt,NULL,producer,NULL);
	    else if(strstr(str,"create consumer")!=NULL)pthread_create(&ct,NULL,consumer,NULL);
	    else if(strstr(str,"exit")!=NULL){close(rfd);end();exit(1);}
	    else if(strstr(str,"list producer")!=NULL)OutputP();
	    else if(strstr(str,"list consumer")!=NULL)OutputC();
	    else if(strstr(str,"delete")!=NULL)deal_delete();
	    else printf("Unkown Command!\n");
	    sleep(5);
	 }					    		
    }

    pthread_join(pt,NULL);
    pthread_join(ct,NULL);
    end();
    return 0;
}
