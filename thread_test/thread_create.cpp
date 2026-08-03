#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void* tfn(void* arg)
{
    printf("thread : pid = %d, tid = %lu\n", getpid(), pthread_self());

    return NULL;
}

int main()
{
    pthread_t tid;

    printf("main : pid = %d, tid = %lu\n", getpid(), pthread_self());
    
    pthread_create(&tid, NULL, tfn, NULL);

    sleep(1);
    
    return 0;
}