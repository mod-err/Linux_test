#include <signal.h>
#include <stdio.h>

void sig_catch(int signal)
{
    printf("---I catch you %d\n", signal);
}

int main()
{
    signal(SIGINT, sig_catch);

    //阻塞程序，预留充足时间发信号
    while(1);s

    return 0;
}