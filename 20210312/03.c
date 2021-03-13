#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stddef.h>

pthread_mutex_t mylock;
int g_var;

void* pthread1(void* arg)
{
	int i;
	printf("I am 1\n");
	pthread_mutex_lock(&mylock);
	for(i=0;i<10;i++)
	{
	   g_var++;
	   printf("th1:%d\n",g_var);
	   sleep(1);
        }
	pthread_mutex_unlock(&mylock);
	return NULL;
}

void* pthread2(void* arg)
{
      int i;
      printf("I am 2\n");
      while(pthread_mutex_trylock(&mylock))
      {
	      printf("I am trying\n");
	      sleep(1);
      }
      for(i=0;i<10;i++)
      {
          g_var--;
	  printf("th2:%d\n",g_var);
	  sleep(1);
      }
      pthread_mutex_unlock(&mylock);
      return NULL;
}

int main()
{
    pthread_t tid1,tid2;
    pthread_mutex_init(&mylock,NULL);
    pthread_create(&tid1,NULL,pthread1,NULL);
    pthread_create(&tid2,NULL,pthread2,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_mutex_destroy(&mylock);
    return 0;
}
