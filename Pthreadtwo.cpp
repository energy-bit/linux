#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define THREADCOUNT 1

void* myStartThread(void* arg)
{
	pthread_detach(pthread_self());
	(void)arg;
	while (1)
	{
		sleep(20);
		printf("i am workthread\n");
		pthread_exit(NULL);
	}
#if 0
	sleep(20);
	printf("i am workthread\n");
	pthread_cancel(pthread_self());
	sleep(5);
	printf("pthread_cancel\n");
	pthread_exit(NULL);
	printf("i am flag\n");
#endif
	return NULL;
}

int main()
{
	pthread_t tid[THREADCOUNT];
	for (int i = 0; i < 1; i++)
	{
		int ret = pthread_create(&tid[i], NULL, myStartThread, NULL);
		if (ret < 0)
		{
			perror("pthread_create");
			return -1;
		}
	}


	for (int i = 0; i < THREADCOUNT; i++)
	{
#if 0 //线程等待
		pthread_join(tid[i], NULL);
#endif

#if 0 //线程分离
		pthread_detach(tid[i]);
#endif
	}

	while (1)
	{
		printf("i am main thread\n");
		sleep(1);
	}

	return 0;
}