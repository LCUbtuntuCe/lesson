#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
	int i;
	sigset_t set,pendset;
	struct sigaction action;

        (void)signal(SIGINT,func_crtl_c);
	if(sigempty(&set)<0)perror("初始化信号集合错误\n");
	if(sigaddset(&set,SIGINT)<0)perror("加入信号集合错误.\n");
	if(sigprocmask(SIG_BLOCK,&set,NULL)<0)perror("往信号阻集增加一个信号集合错误.\n");
        else
	{
		for(i=0;i<5;i++)
		{
			printf("显示此文字,表示程序处于阻塞信号状态.\n");
		}
	}

	if(sigprocmask(SIG_UNBLOCK,&set,NULL)<0)perror("从信号阻塞集删除一个信号集合错误\n");

	return 0;
}

void fun_ctrl_c()
{
	printf("\t你按了ctrl+c，系统是不是很长时间没理你.\n");
	printf("\t信号处理函数: 有什么要处理，在处理函数中编程！\n");
	printf("\t此例不处理，直接结束！\n");
	(void)signal(SIGINT,SIG_DFL);
}
