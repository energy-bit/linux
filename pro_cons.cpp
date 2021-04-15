#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <queue>
#include <unistd.h>
#include <sys/syscall.h>

#define RESORCECOUNT 4
#define THREADCOUT 2
class RingQueue
{
    private:
        std::queue<int> que_;
        pthread_mutex_t lock_;
        pthread_cond_t con_;
        pthread_cond_t pro_;
        int Capacity_;
    public:
        RingQueue()
            :Capacity_(RESORCECOUNT)
        {
            pthread_mutex_init(&lock_,NULL);
            pthread_cond_init(&pro_,NULL);
            pthread_cond_init(&con_,NULL);
        }
        ~RingQueue()
        {
            pthread_mutex_destroy(&lock_);
            pthread_cond_destroy(&pro_);
            pthread_cond_destroy(&con_);
        }
        

        void Push(int data)
        {
            pthread_mutex_lock(&lock_);
            while((int)que_.size() == Capacity_) {
                pthread_cond_wait(&pro_, &lock_);
            }

            que_.push(data);
            pthread_mutex_unlock(&lock_);
            pthread_cond_signal(&con_);
        }
        void Pop(int &data)
        {
            pthread_mutex_lock(&lock_);
            while(que_.empty()) {
                pthread_cond_wait(&con_, &lock_);
            }
            data = que_.front();
            que_.pop();
            pthread_mutex_unlock(&lock_);
            pthread_cond_signal(&pro_);
        }
};

void* consStart(void* arg)
{
    RingQueue *cp = (RingQueue *)arg;
    int data = 0;
    while(1)
    {
        cp->Pop(data);
        printf(" i am pid %d,i get data %d\n",SYS_gettid,data);
    }
}

void* proStart(void* arg)
{
   RingQueue *cp = (RingQueue *)arg;
   int data = 0;
   while(1)
   {
       cp->Push(data);
       printf("i am pid %d,i push data:%d\n",SYS_gettid,data);
       data++;
   }

}

int main()
{
    RingQueue *cp = new RingQueue;
    pthread_t cons[THREADCOUT],pro[THREADCOUT];

    for(int i = 0;i < THREADCOUT;++i){
        int ret=pthread_create(&cons[i],NULL,consStart,(void*)cp);
        if(ret<0){
            perror("thread create error");
            return -1;
        }
        ret = pthread_create(&pro[i],NULL,proStart,(void*)cp);
        if(ret<0){
            perror("thread create error");
            return -1;
        }
    }

    for(int i = 0;i < THREADCOUT;++i){
        pthread_join(cons[i],NULL);
        pthread_join(pro[i],NULL);
    }
    return 0;
}
