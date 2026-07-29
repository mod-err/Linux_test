#include <signal.h>
#include <stdio.h>

int main()
{
    sigset_t set;

    sigemptyset(&set); //将自己创建的信号集清理

    sigaddset(&set, SIGINT); //将2号SIGINT信号[ctrl + c] 加入信号集
    sigaddset(&set, SIGQUIT); //将3号SIGQUIT信号[ctrl + \] 加入信号集
    sigaddset(&set, SIGBUS); //将7号总线信号加入信号集
    sigaddset(&set, SIGKILL); //将9号KILL信号加入信号集，9号信号无法被屏蔽

    sigprocmask(SIG_BLOCK, &set, NULL); //屏蔽信号
    
    sigset_t pendset; //传出参数

    //循环打印未决信号集
    int i;
    int bit;
    while(1)
    {
        sigpending(&pendset); //信号被阻塞后变为未决态，查看未决信号集

        for(i = 1; i < 32; i++)
        {
            bit = sigismember(&pendset, i);
            printf("%d", bit);
        }
        printf("\n");

        sleep(1);
    }

}