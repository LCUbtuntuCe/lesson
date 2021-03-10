#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t result;
	int r_num;
	int pipe_fd[2];
	char buf_r[100];
	memset(buf_r,0,sizeof(buf_r));
	if(pipe(pipe_fd)<0)
	{
		printf("failed to build a pipe!\n");
		return -1;
	}
	result=fork();
	if(result<0)
	{
		perror("build its son failed!\n");
		exit;
	}
	else if(result==0)
	{
		close(pipe_fd[1]);
		if(r_num=read(pipe_fd[0],buf_r,100)>0)printf("the son have read the %d data from the pipe,and the string is %s\n",r_num,buf_r);
		close(pipe_fd[0]);
		exit(0);
	}
	else
	{
		if(write(pipe_fd[1],"the first string",10)!=-1)
			printf("the father write ths first string to the pipe!\n");
		if(write(pipe_fd[1],"the second string",10)!=-1)
			printf("the father write ths first string to the pipe!\n");
		close(pipe_fd[1]);
		waitpid(result,NULL,0);
		exit(0);
	}
	return 0;
}
