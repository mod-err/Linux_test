#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <pthread.h>
#include <error.h>
#include "wrap.h"

#define SERV_PORT 9527

//线程用于通信
void *do_work(void* arg)
{
    int ret;
    char buf[4096]; //接收数据的缓冲区
    int cfd = (int)(long)arg; //client文件描述符

    //子进程进行while(1)循环，直到Read返回0客户端断开才break退出
    while(1)
    {
        ret = Read(cfd, buf, sizeof(buf)); //ret保存读取到的字节数，一次调用能读多少读多少，不会超过buf的大小
        //如果ret = 0就说明客户端关闭了连接，此时关闭clientfd退出进程
        if(ret == 0)
        {
            close(cfd);
            return NULL; //线程退出用return
        }
        Write(STDOUT_FILENO, buf, ret);  //在终端上显示收到的数据

        for(int i = 0; i < ret; i++)
        {
            buf[i] = toupper(buf[i]); //逐个字符进行转换
        }
        Write(cfd, buf, ret); //向文件描述符为clientfd的文件，写入同样字节大小的数据
    }
}

int main(int argc, char* argv[])
{
    int listenfd = 0, clientfd = 0; //listenfd监听套接字文件描述符，clientfd通信套接字

    pthread_t tid;  //接收线程ID

    //各类协议万能壳
    struct sockaddr_in serv_addr, clit_addr;

    bzero(&serv_addr,sizeof(serv_addr));  //⭐ 将地址结构清零，防止栈垃圾
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); //主机字节序转网络字节序
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //本机IP，同样转为网络字节序

    socklen_t clit_addr_len; //接收客户端的协议大小

    //1.创建Socket套接字
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    //2.绑定端口和IP
    Bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //3.设置与服务器同时连接的客户端上限
    Listen(listenfd, 128);

    //4.子线程用于通信，主线程监听并回收子线程
    while(1)
    {
        // ⭐ 每次循环前重置长度
        clit_addr_len = sizeof(clit_addr);
        //阻塞等待客户端连接
        clientfd = Accept(listenfd, (struct sockaddr *)&clit_addr, &clit_addr_len);
        
        //创建线程
        int tmp = pthread_create(&tid, NULL, do_work, (void*)(long)clientfd);
        //线程创建错误处理
        if(tmp != 0)
        {
            printf("thread detach error : %s\n", strerror(tmp));
        } 
        //分离子线程：线程终止会自动回收
        pthread_detach(tid);
    }
}