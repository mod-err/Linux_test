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
    pthread_t tid;
    pthread_attr_t attr;

    //初始化线程属性
    int ret = pthread_attr_init(&attr);
    if(ret != 0)
    {
        printf("pthread_attr_init error : %s\n", strerror(ret));
    }
    //设置分离态
    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(ret != 0)
    {
        printf("pthread_attr_setdetachstate error : %s\n", strerror(ret));
    }
    //创建线程
    ret = pthread_create(&tid, &attr, tfn, NULL);
    if(ret != 0)
    {
        printf("pthread_create error : %s\n", strerror(ret));
    }
    //销毁用于初始化的结构体
    ret = pthread_attr_destroy(&attr);
    if(ret != 0)
    {
        printf("pthread_attr_destroy error : %s\n", strerror(ret));
    }

    /*-------主进程任务--------*/
    printf("main thread : pid = %d, tid = %lu\n", getpid(), pthread_self());

    //尝试回收子线程：子线程已经被分离，不应该主线程被回收。这里只是为了测试是否分离
    ret = pthread_join(tid, NULL);
    if(ret != 0)
    {
        printf("pthread_join error : %s\n", strerror(ret));
    }

    /*-------------------------*/

    //主进程退出
    pthread_exit(NULL);
    return 0;
}
