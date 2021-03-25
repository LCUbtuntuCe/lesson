#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>
#include<sys/types.h>

int main()
{
	int counter;
	pid_t pid;
	(void)signal(SIGTSTP,SIG_IGN);
	(void)signal(SIGTSTP,SIG_DFL);
	pid=fork();
	if(pid<0)
	{
		perror("fork failed!\n");
		exit(-1);
	}
	else if(pid==0)
	{
		while(1)
	        {
		    counter++;
		    printf("running %ds\n",counter);
		    sleep(1);
	        }
		exit(0);
        }
	else{
		sleep(10);
		signal(pid,SIGKILL);
	}
	return 0;
}

