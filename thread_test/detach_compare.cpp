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
    
#if 1
    pthread_attr_init(&attr); //初始化线程属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //设置分离态
    pthread_create(&tid, &attr, tfn, NULL); //创建线程
    pthread_attr_destroy(&attr); //销毁用于初始化的结构体
#else
    pthread_create(&tid, &attr, tfn, NULL); //创建线程
    pthread_detach(tid);
#endif

    /*-------主进程任务--------*/
    printf("main thread : pid = %d, tid = %lu\n", getpid(), pthread_self());

    //尝试回收子线程：子线程已经被分离，不应该主线程被回收。这里只是为了测试是否分离
    int ret = pthread_join(tid, NULL);
    if(ret != 0)
    {
        printf("pthread_join error : %s\n", strerror(ret));
    }

    /*-------------------------*/

    //主进程退出
    pthread_exit(NULL);
    return 0;
}
