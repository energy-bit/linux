#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define THREADCOUNT 4

int g_tickets = 100;
pthread_mutex_t lock_;

void* MyStartThread(void* arg)
{
    (void)arg;
    while(1)
    {
        pthread_mutex_lock(&lock_);
        if(g_tickets > 0)
        {
            printf("i am thread %p, i have ticket %d\n", pthread_self(), g_tickets);
            g_tickets--;
            sleep(5);
            int* lp = NULL;
            *lp = 10;
        }
        else
        {
            //pthread_mutex_unlock(&lock_);
            break;
        }
        //pthread_mutex_unlock(&lock_);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&lock_, NULL);
    pthread_t tid[THREADCOUNT];
    for(int i = 0; i < THREADCOUNT; i++)
    {
        int ret = pthread_create(&tid[i], NULL, MyStartThread, NULL);
        if(ret < 0)
        {
            perror("pthread_create");
            return -1;
        }
    }

    for(int i = 0; i < THREADCOUNT; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&lock_);
    return 0;
}
