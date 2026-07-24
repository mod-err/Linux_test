#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int main()
{
    //创建管道
    int fd[2];
    int ret;

    ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe error");
        exit(1);
    }

    //创建子进程
    pid_t pid;
    int i;

    for(i = 0; i < 2; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            break;
        }
    }

    //父进程
    if(i == 2)
    {
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        wait(NULL);
    }
    else if(i == 1) //兄进程
    {
        close(fd[0]); //关闭读端
        //重定向标准输出到管道输入端，写数据到管道输入
        dup2(fd[1], STDOUT_FILENO); 
        //ls原本输出数据到终端
        execlp("ls", "ls", NULL);

        perror("execlp error");
        exit(1);
    }
    else if(i == 0) //弟进程
    {
        close(fd[1]); //关闭写端
        //重定向标准输入到管道输出端，从管道输出读取数据
        dup2(fd[0], STDIN_FILENO); 
        //wc原本从键盘输入读数据
        execlp("wc", "wc", "-l", NULL);

        perror("execlp error");
        exit(1);
    }
    
}