#include<stdio.h>
#include<unistd.h>

int main(int argc,const char* argv[])
{
        argv={"usr/bin/vim",NULL};
	printf("系统分配的进程号（PID）是：%d",getpid());
	if(execve("usr/bin/vim",*argv,NULL)<0)
	{
		perror("用execve创建进程时出错\n");
	}
	return 0;
}
