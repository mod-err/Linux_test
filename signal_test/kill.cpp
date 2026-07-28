#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int main()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        printf("child pid : %d\n", getpid());
        sleep(2);
        kill(getppid(), SIGKILL); //杀死阻塞的父进程
        
    }
    else if(pid > 0)
    {
        printf("parent pid : %d\n", getpid());
        while(1);
    }
    return 0;
}