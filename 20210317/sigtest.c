#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

int main()
{
	int counter;
	while(1)
	{
		counter++;
		printf("running %ds\n",counter);
		sleep(1);
	}
	return 0;
}
