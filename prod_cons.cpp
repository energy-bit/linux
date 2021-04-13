#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>

#define THREADCOUNT 2

//生产者与消费者模型
//   1.线程安全的队列
//      std::queue
//      互斥： pthread_mutex_t
//      同步：pthread_cond_t
//   2.两种的角色的线程
//      生产者线程 --》生产者线程的入口函数
//      消费线程   --》消费线程的入口函数

class BlockQueue
{
    public:
        BlockQueue()
        {
            capacity_ = 10;
            pthread_mutex_init(&lock_, NULL);
            pthread_cond_init(&prod_cond_, NULL);
            pthread_cond_init(&cons_cond_, NULL);
        }

        ~BlockQueue()
        {
            pthread_mutex_destroy(&lock_);
            pthread_cond_destroy(&cons_cond_);
            pthread_cond_destroy(&prod_cond_);
        }

        //写多线程代码的时候
        //   1.考虑业务核心逻辑
        //   2.考虑核心逻辑当中是否访问临界资源或者说执行临界区代码, 如果有需要保证互斥
        //   3.需不需要保证各个线程之间同步
        //生产者线程调用
        void Push(int data)
        {
            pthread_mutex_lock(&lock_);
            while(que_.size() >= capacity_)
            {
                pthread_cond_wait(&prod_cond_, &lock_);
            }

            que_.push(data);
            pthread_mutex_unlock(&lock_);

            pthread_cond_signal(&cons_cond_);
        }

        //消费者线程调用的
        void Pop(int* data)
        {
            pthread_mutex_lock(&lock_);
            while(que_.empty())
            {
                pthread_cond_wait(&cons_cond_, &lock_);
            }
            *data = que_.front();//拿队列首部元素的值
            que_.pop();//出队操作
            pthread_mutex_unlock(&lock_);

            pthread_cond_signal(&prod_cond_);
        }
    private:
        //STL当中的queue是不安全的
        std::queue<int> que_;
        //队列设置一个容量， 不能无节制的让队列一直进行扩容， 有可能会导致我们申请内存失败或者当前程序被操作系统干掉
        size_t capacity_;

        //保证STL当中的queue同步和互斥
        pthread_mutex_t lock_;
        pthread_cond_t prod_cond_;
        pthread_cond_t cons_cond_;
};

void* ConsumeStart(void* arg)
{
    BlockQueue* bq = (BlockQueue*)arg;
    while(1)
    {
        //从线程安全队列当中获取数据进行消费
        int data;
        bq->Pop(&data);
        printf("i am %p, i consume %d\n", pthread_self(), data);
    }
    return NULL;
}

void* ProductStart(void* arg)
{
    BlockQueue* bq = (BlockQueue*)arg;
    //如果是多个线程， 每个线程都会在自己独有的栈当中压栈该入口函数， data为临时变量， 每一个线程都是拥有一个data这样的临时
    int data = 0;
    while(1)
    {
        //往线程安全队列当中插入数据
        bq->Push(data);
        //调用完毕Push之后， 互斥锁就释放了
        //这会有可能该执行流时间片到了， 线程切换出去了
        //printf并不是原子性的
        printf("i am %p, i product %d\n", pthread_self(), data);
        data++;
    }
    return NULL;
}

int main()
{
    pthread_t cons[THREADCOUNT], prod[THREADCOUNT];

    BlockQueue* bq = new BlockQueue();

    for(int i = 0;  i < THREADCOUNT; i++)
    {
        int ret = pthread_create(&cons[i], NULL, ConsumeStart, (void*)bq);
        if(ret < 0)
        {
            perror("pthread_create");
            return -1;
        }

        ret = pthread_create(&prod[i], NULL, ProductStart, (void*)bq);
        if(ret < 0)
        {
            perror("pthread_create");
            return -1;
        }
    }

    for(int i = 0; i < THREADCOUNT; i++)
    {
        pthread_join(cons[i], NULL);
        pthread_join(prod[i], NULL);
    }
    return 0;
}
