#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

char buff[1024];

void sig_catch(int sig)
{
    pid_t wpid;
    while((wpid = wait(NULL)) != -1)
    {
        sprintf(buff, "waitpid child : %d\n", wpid);
        write(STDOUT_FILENO, buff, strlen(buff));
    }  
}

int main(int argc, char* argv[])
{
    int n = 5; //子进程个数
    pid_t pid; //接收创建进程的ID
    int i;

    //阻塞信号
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD); //SIGCHLD加入信号集
    sigprocmask(SIG_BLOCK, &set, NULL); //屏蔽信号

    for(i = 0; i < n; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            break;
        }
    }

    if(i == n)
    {
        struct sigaction act;

        act.sa_handler = sig_catch; //设置回调函数
        sigemptyset(&act.sa_mask);   //不屏蔽任何信号
        act.sa_flags = 0;           //默认设置

        sigaction(SIGCHLD, &act, NULL);

        //解除阻塞
        sigprocmask(SIG_UNBLOCK, &set, NULL); 

        sprintf(buff, "parent process : %d\n", getpid());
        write(STDOUT_FILENO, buff, strlen(buff));

        while(1);
    }
    else 
    {
        //sleep(1); //先让子进程休眠1s，避免父进程的进程回收还未初始化子进程就退出
        sprintf(buff, "child process : %d\n", getpid());
        write(STDOUT_FILENO, buff, strlen(buff));
    }
}