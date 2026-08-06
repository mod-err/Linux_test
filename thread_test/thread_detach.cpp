#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <string.h>

void* tfn(void* arg)
{
    printf("thread : pid = %d, tid = %lu\n", getpid(), pthread_self());

    return NULL;
}

int main()
{
    //接收子线程的线程ID
    pthread_t tid;
    
    int ret = pthread_create(&tid, NULL, tfn, NULL);
    if(ret != 0)
    {
        printf("thread create error : %s\n", strerror(ret));
    }

    printf("main : pid = %d, main_tid = %lu\n", getpid(), pthread_self());

    //分离子线程：线程终止会自动回收
    ret = pthread_detach(tid);
    if(ret != 0)
    {
        printf("thread detach error : %s\n", strerror(ret));
    }
    
    //回收子线程
    ret = pthread_join(tid, NULL);
    if(ret != 0)
    {
        printf("pthread_join return %d\n", ret);
        printf("thread join error : %s\n", strerror(ret));
    }

    //主进程正常退出
    pthread_exit(NULL);

    return 0;
}