#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void* func()
{
    return NULL;
}

void* tfn(void* arg)
{
    int i = (int)(long)arg;
    sleep(i); 

    if(i == 2)
    {
        //exit(0);      //正常退出进程
        //return NULL;  //返回到调用者，退出当前线程
        //func();       //退出func()函数
        //pthread_exit(NULL); //退出当前线程
    }

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
   
    sleep(i); 

    printf("main : pid = %d, tid = %lu\n", getpid(), pthread_self());

    //pthread_exit(NULL);

    return 0;
}
