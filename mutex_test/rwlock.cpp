#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int counter;

//全局锁
pthread_rwlock_t rwlock;

void* th_write(void* arg)
{
    int t;
    int i = (int)(long)arg;
    while(1)
    {
        t = counter;
        sleep(1);

        //[2] 加读锁
        pthread_rwlock_rdlock(&rwlock);

        printf("======write %d : tid = %lu : counter = %d, ++counter = %d\n", i, pthread_self(), t, ++counter); //counter前后对比

        //[3] 解读锁
        pthread_rwlock_unlock(&rwlock);

        sleep(9); //给读锁提供机会
    }
    return NULL;
}

void* th_read(void* arg)
{
    int i = (int)(long)arg;
    while(1)
    {
        //[2] 加写锁
        pthread_rwlock_wrlock(&rwlock);

        printf("======read %d : tid = %lu : counter = %d\n", i, pthread_self(), counter); //counter前后对比

        //[3] 加写锁
        pthread_rwlock_unlock(&rwlock);

        sleep(2); //给写锁提供机会
    }
    return NULL;
}

int main()
{
    int i;
    pthread_t tid[8];

    //[1] 初始化互斥锁
    pthread_rwlock_init(&rwlock, NULL);

    for(i = 0; i < 3; i++)
    {
        pthread_create(&tid[i], NULL, th_write, (void*)(long)i);
    }
    for(i = 0; i < 5; i++)
    {
        pthread_create(&tid[i+3], NULL, th_read, (void*)(long)i);
    }

    //循环回收子线程
    for(i = 0; i < 8; i++)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}