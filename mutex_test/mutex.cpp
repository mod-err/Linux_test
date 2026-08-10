#include <pthread.h>
#include <stdlib.h>  //srand
#include <unistd.h>  //sleep
#include <stdio.h>   //printf
#include <string.h>  //strerror

//创建互斥锁
pthread_mutex_t mutex;

void* tfn(void* arg)
{
    srand(time(NULL));

    while(1)
    {
        //[2] 加锁
        pthread_mutex_lock(&mutex);

        printf("hello ");
        sleep(rand() % 3);
        printf("world\n");

        //[3] 解锁
        pthread_mutex_unlock(&mutex);
        
        sleep(rand() % 3);
    }

    return NULL;
}

int main()
{
    //[1] 初始化互斥锁
    pthread_mutex_init(&mutex, NULL); 
    
    pthread_t tid;
    pthread_create(&tid, NULL, tfn, NULL);

    srand(time(NULL));

    while(1)
    {
        //[2] 加锁
        pthread_mutex_lock(&mutex);

        printf("HELLO ");
        sleep(rand() % 3);
        printf("WORLD\n");

        //[3] 解锁
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3);
    }

    //回收子线程
    int ret = pthread_join(tid, NULL); 
    if(ret != 0)
    {
        printf("thread join error : %s\n", strerror(ret));
    }

    //[4] 销毁锁
    pthread_mutex_destroy(&mutex);

    return 0;
}