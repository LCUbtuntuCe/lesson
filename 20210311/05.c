#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

typedef struct{
	char name[4];
	int age;
}people;

int main()
{
	int shm_id,i;
	key_t key;
	people* p_map;
	char* name="~/kkk";
	key=ftok(name,0);
	shm_id=shmget(key,4096,IPC_CREAT);
	if(shm_id==-1)
	{
		perror("获取共享内存区域的ID出错.\n");
		return 0;
	}
	p_map=(people*)shmat(shm_id,NULL,0);
	for(i=0;i<10;i++)
	{
		printf("NAME: %s\t",(*(p_map+i)).name);
		printf("AGE: %d\n",(*(p_map+i)).age);
	}
	if(shmdt(p_map)==-1)perror("解除映射时出错！\n");
	return 0;
}
