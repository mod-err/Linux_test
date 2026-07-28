#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void myfunc(int s)
{
    printf("hello world\n");
}

int main()
{
    signal(SIGALRM, myfunc);

    struct itimerval it, oldit;
    //定时间隔时间
    it.it_interval.tv_sec = 2;
    it.it_interval.tv_usec = 0;
    //起始定时时间
    it.it_value.tv_sec = 5;
    it.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &it, NULL);

    if(ret == -1)
    {
        perror("setitimer error");
        exit(1);
    }

    //避免还没到定时时间，程序直接退出
    while(1); 
}