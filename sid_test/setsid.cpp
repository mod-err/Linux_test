#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid = fork();

    //子进程
    if(pid == 0)
    {
        printf("child process PID : %d\n", getpid());
        printf("child process PGID : %d\n", getpgid(0));
        printf("child process SID : %d\n", getsid(0));

        sleep(10);
        setsid(); //设置会话

        printf("Changed: \n");
        printf("child process PID : %d\n", getpid());
        printf("child process PGID : %d\n", getpgid(0));
        printf("child process SID : %d\n", getsid(0));
    }

    //父进程直接退出
    return 0;
}