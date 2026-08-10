#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void* tfn(void* arg)
{
    while(1)
    {
        printf("thread pid = %d, tid = %lu\n", getpid(), pthread_self());
        sleep(1);

        //pthread_testcancel(); //设置取消点
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t tid;

    pthread_create(&tid, NULL, tfn, NULL);
    
    //主进程执行任务
    printf("main : pid = %d, tid = %lu\n\n", getpid(), pthread_self());

    sleep(5);

    pthread_cancel(tid); //杀死子线程

    pthread_join(tid, NULL); //阻塞等待回收子线程，并且不关心退出状态

    //退出主进程
    pthread_exit(NULL);

    return 0;
}