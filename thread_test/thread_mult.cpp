#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void* tfn(void* arg)
{
    int i = (int)(long)arg;
    sleep(i); //子线程按顺序打印，防止抢占CPU
    printf("I am %dth thread : pid = %d, tid = %lu\n", i+1, getpid(), pthread_self());

    return NULL;
}

int main()
{
    pthread_t tid;
    int i;

    for(i = 0; i < 5; i++)
    {
        pthread_create(&tid, NULL, tfn, (void*)(long)i);
    }
    
    sleep(i); //主进程延时5s，保证子线程全部执行完成

    printf("main : pid = %d, tid = %lu\n", getpid(), pthread_self());
    
    return 0;
}