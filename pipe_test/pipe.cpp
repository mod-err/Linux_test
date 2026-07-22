#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>

using namespace std;

int main()
{
    //先创建管道
    int ret;
    int fd[2];

    ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe error");
        exit(1);
    }

    //再创建子进程
    pid_t pid;

    pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
        
    //父进程写，子进程读
    const char* str = "hello pipe\n";
    char buf[1024];

    if(pid > 0)  //父进程
    {
        close(fd[0]); //关闭读端
        write(fd[1], str, strlen(str));
        close(fd[1]);
    }
    else if(pid == 0)  //子进程
    {
        close(fd[1]);//关闭写端
        ret = read(fd[0], buf, sizeof(buf)); //写入buf缓冲区中
        cout << "child process read size : " << ret << endl;
        write(STDOUT_FILENO, buf, ret); //cout << buf << endl; //从缓冲区写入到终端
        close(fd[0]); 
    }
}