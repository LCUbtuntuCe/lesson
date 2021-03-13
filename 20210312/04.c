#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

sem_t mysem,my_space;
int buf[10];
int idx=0;

void* producer(void* arg)
{
	int i;
	for(i=0;i<100;i++)
	{
		sem_wait(&my_space);
		idx++;
		if(i==99)
		{
			buf[idx]=0xab;
		}
		else buf[idx]=rand();

		printf("product:%d\n",buf[idx]);
		sem_post(&mysem);
		sleep(1);
	}
	return NULL;
}

void *consumer(void* arg)
{
     while(1)
     {
	     sem_wait(&mysem);
	     printf("consumer:%d\n",buf[idx]);
	     if(buf[idx]==0xab)break;
	     idx--;
	     sem_post(&my_space);
	     sleep(1);
     }
     return NULL;
}

int main()
{
	pthread_t pid1,pid2;
	sem_init(&mysem,0,0);
	sem_init(&my_space,0,10);
	pthread_create(&pid1,NULL,producer,NULL);
	pthread_create(&pid2,NULL,consumer,NULL);
        pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	sem_destroy(&mysem);
	return 0;
}
