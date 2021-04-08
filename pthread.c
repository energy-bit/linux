#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>

void* TStart(void* arg)
{
	while (1)
	{
		printf("i am work thread\n");
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, TStart, NULL);
	if (ret < 0)
	{
		perror("pthread_create");
		return -1;
	}

	while (1)
	{
		printf("i am main thread\n");
		sleep(1);
	}
	return 0;
}



#if 0
volatile int g_val = 1;

void sigcb(int num)
{
	printf("num : %d\n", num);
	g_val = 0;
}

int main()
{

	signal(2, sigcb);
	while (g_val)
	{
	}
	return 0;
}
#endif

#if 0
void sigcb(int signo)
{
	wait(NULL);
	printf("wait..done\n");
}

int main()
{
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return -1;
	}
	else if (pid == 0)
	{
		//child   ==> getpid();
		sleep(5);
		exit(0);
	}
	else
	{
		signal(SIGCHLD, sigcb);
		while (1)
		{
			printf("hello world\n");
			sleep(1);
		}
	}
	return 0;
}
#endif