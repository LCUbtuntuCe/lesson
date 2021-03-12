#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stddef.h>

void *my_print(void* arg)
{
	int i;
	//为保证线程安全,也就是让程序运行在可控范围内，不要用全局或静态局部变量
	static int n=0;
	for(i=0;i<10;i++)
	{
		printf("%c%d\n",(char)(long)arg,++n);
		fflush(stdout);
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t tid1,tid2;
        pthread_create(&tid1,NULL,my_print,(void*)'#');
	pthread_create(&tid2,NULL,my_print,(void*)'*');
 	pthread_join(tid1,NULL);
	pthread_join(tid1,NULL);
	return 0;
}
