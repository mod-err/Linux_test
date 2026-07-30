#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void sig_catch(int sig)
{
    //printf("catch SIGINT -%d\n", sig);
    const char* buf;
    if(sig == SIGINT)
    {
        buf = "catch SIGINT\n";
    }
    /*else if(sig == SIGQUIT)
    {
        buf = "catch SIGQUIT\n";
    }*/ 
    write(STDOUT_FILENO, buf, strlen(buf));  
    sleep(10);
}

int main()
{
    struct sigaction act;

    act.sa_handler = sig_catch; //设置回调函数
    sigemptyset(&act.sa_mask);  //清空sa_mask屏蔽字，只在sig_catch期间有效
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_flags = 0;           //默认值

    sigaction(SIGINT, &act, NULL);  //2号
    //sigaction(SIGQUIT, &act, NULL); //3号

    while(1);
}
