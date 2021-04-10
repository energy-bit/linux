#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define THREADCOUNT 2

int g_bowl = 0;
pthread_mutex_t g_mut;
pthread_cond_t g_cond;

void* EatStart(void* arg)
{
	while (1)
	{
		//eat
		pthread_mutex_lock(&g_mut);
		while (g_bowl <= 0)
		{
			pthread_cond_wait(&g_cond, &g_mut);
		}
		printf("i am %p, i eat %d\n", pthread_self(), g_bowl);
		g_bowl--;
		pthread_mutex_unlock(&g_mut);
		//通知做面的人做面
		pthread_cond_signal(&g_cond);
	}
	return NULL;
}

void* MakeStart(void* arg)
{
	while (1)
	{
		//make
		pthread_mutex_lock(&g_mut);
		while (g_bowl > 0)
		{
			//等待
			pthread_cond_wait(&g_cond, &g_mut);
		}
		g_bowl++;
		printf("i am %p, i make %d\n", pthread_self(), g_bowl);
		pthread_mutex_unlock(&g_mut);
		pthread_cond_signal(&g_cond);
	}
	return NULL;
}

int main()
{
	pthread_mutex_init(&g_mut, NULL);
	pthread_cond_init(&g_cond, NULL);
	pthread_t consume[THREADCOUNT], product[THREADCOUNT];
	for (int i = 0; i < THREADCOUNT; i++)
	{
		int ret = pthread_create(&consume[i], NULL, EatStart, NULL);
		if (ret < 0)
		{
			perror("pthread_create");
			return -1;
		}

		ret = pthread_create(&product[i], NULL, MakeStart, NULL);
		if (ret < 0)
		{
			perror("pthread_create");
			return -1;
		}
	}

	for (int i = 0; i < THREADCOUNT; i++)
	{
		pthread_join(consume[i], NULL);
		pthread_join(product[i], NULL);
	}

	pthread_mutex_destroy(&g_mut);
	pthread_cond_destroy(&g_cond);
	return 0;
}