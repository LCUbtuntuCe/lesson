#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<string.h>
#include<signal.h>
#include <errno.h>

typedef struct{
	char name[4];
	int age;
}people;

int main()
{
	int shm_id,i;
	key_t key;
	char temp;
	people *p_map;
	char* name="~/kkk";
	key=ftok(name,0);
	shm_id=shmget(key,4096,IPC_CREAT);
	if(shm_id==-1)
	{
		perror("获取共享内存区域的ID出错");
		return 0;
	}
	printf("%s\n",strerror(errno));
	p_map=(people*)shmat(shm_id,NULL,0);
	temp='a';
	for(i=0;i<10;i++)
	{
		temp+=1;
		memcpy((*(p_map+i)).name,&temp,1);
		(*(p_map+1)).age=20+i;
	}
	if(shmdt(p_map)==-1)perror("解除映射时出错\n");
	return 0;
}
