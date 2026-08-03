#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int val = 100;

void* tfn(void* arg)
{
    val = 200;
    printf("thread : global val = %d\n", val);

    return NULL;
}

int main()
{
    printf("main : global val = %d\n", val);

    pthread_t tid;
    pthread_create(&tid, NULL, tfn, NULL);

    sleep(1);

    printf("main : global val = %d\n", val);
    
    return 0;
}