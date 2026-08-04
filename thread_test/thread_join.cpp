#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void* tfn(void* arg)
{
    //在堆区开辟存储空间
    int* arr = new int[3];
    for(int i = 0; i < 3; i++)
    {
        arr[i] = i + 100;
    }

    printf("thread : pid = %d, tid = %lu\n", getpid(), pthread_self());
    //返回指针类型
    return (void*)arr;
}

int main()
{
    pthread_t tid;

    printf("main : pid = %d, tid = %lu\n", getpid(), pthread_self());
    
    pthread_create(&tid, NULL, tfn, NULL);

    /*-----主进程任务-----*/
    int* p;
    //阻塞等待线程回收
    pthread_join(tid, (void**)&p); // 取p的地址，传进去让内核修改p的值
    printf("child thread exit with arr[0] = %d, arr[1] = %d, arr[2] = %d\n", p[0], p[1], p[2]);

    //主进程正常退出
    pthread_exit(NULL);
    
    delete[] p;

    return 0;
}