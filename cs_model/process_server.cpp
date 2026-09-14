#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h> 
#include "wrap.h"

#define SERV_PORT 9527

//信号回调函数
void sig_catch(int sig)
{
    //非阻塞回收子进程
    while((waitpid(0, NULL, WNOHANG)) > 0);
    return;
}

//实现多进程并发服务器，可以支持多个客户端连接
int main(int argc, char* argv[])
{
    pid_t pid; //进程ID
    int ret;
    char buf[4096]; //接收数据的缓冲区

    //lfd为服务器创建的Socket文件描述符，cfd是接收的到客户端Socket的文件描述符
    int lfd = 0, cfd = 0;

    //各类协议万能壳
    struct sockaddr_in serv_addr, clit_addr;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); //主机字节序转网络字节序
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //本机IP，同样转为网络字节序

    socklen_t clit_addr_len; //接收客户端的协议大小

    //1.创建Socket套接字
    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    //2.绑定端口和IP
    Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //3.设置与服务器同时连接的客户端上限
    Listen(lfd, 128);

    //4.子进程用于通信，父进程监听并回收子进程
    clit_addr_len = sizeof(clit_addr);
    while(1)
    {
        //阻塞等待客户端连接
        cfd = Accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);
        //创建子进程
        pid = fork();
        if(pid < 0) //(1)创建失败
        {
            perror("fork error!");
            exit(1);
        } 
        else if(pid == 0) //(2)子进程
        {
            close(lfd); //子进程只用通信，不用监听套接字：关闭用于监听的套接字
            break; //子进程通信代码放置在循环外或者这里等价
        }
        else //(3)父进程：回收子进程
        {
            //信号捕捉函数回收子进程
            struct sigaction act;
            act.sa_handler = sig_catch; //设置回调函数
            sigemptyset(&act.sa_mask); //清空sa_mask屏蔽字，只在sig_catch期间有效
            act.sa_flags = 0; 
            sigaction(SIGCHLD, &act, NULL);  //注册

            close(cfd); //关闭用于通信的套接字
            continue;
        }
    }

    //子进程
    if(pid == 0)
    {
        //子进程进行while(1)循环，直到Read返回0客户端断开才break退出
        while(1)
        {
            ret = Read(cfd, buf, sizeof(buf)); //ret保存读取到的字节数，一次调用能读多少读多少，不会超过buf的大小
            //如果ret = 0就说明客户端关闭了连接，此时关闭cfd退出进程
            if(ret == 0)
            {
                close(cfd);
                exit(1);
            }
            Write(STDOUT_FILENO, buf, ret);  //在终端上显示收到的数据

            for(int i = 0; i < ret; i++)
            {
                buf[i] = toupper(buf[i]); //逐个字符进行转换
            }
            Write(cfd, buf, ret); //向文件描述符为cfd的文件，写入同样字节大小的数据
        }
    }
}