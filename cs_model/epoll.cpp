#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <string.h> //memset
#include <stdlib.h> //exit
#include <stdio.h>  //perror
#include <unistd.h>
#include <ctype.h> //toupper
#include "wrap.h"

#define SERV_PORT 9527

int main(int argc, char* argv[])
{
    int lfd = 0, cfd = 0; //保存socket文件的文件描述符

    char buf[4096], client_IP[1024]; //接收客户端点分十进制IP地址

    struct sockaddr_in serv_addr, clit_addr;
    socklen_t clit_addr_len;

    //把serv_addr这个变量所占的全部内存字节清零
    memset(&serv_addr, 0, sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); //主机字节序转为网络字节序
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //本机IP，同样转为网络字节序

    //1.创建socket套接字
    lfd = Socket(AF_INET, SOCK_STREAM, 0); 

    //2.绑定端口和IP
    Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //3.设置与服务器同时连接的客户端上限
    Listen(lfd, 128);

    /*----------------初始化epoll------------------*/
    int efd = 0; //epoll实例的文件描述符
    efd = epoll_create(1024); //传入参考监听节点数量1024
    if(efd == -1)
    {
        perror("epoll create error!");
        exit(1); //非零值：程序异常退出
    }

    struct epoll_event tep; //epoll_ctl参数：描述监听事件与附带数据的结构体
    struct epoll_event ep[1024]; //epoll_wait参数：放置存在事件的结构体，ep为结构体数组

    //初始化tep结构体
    tep.events = EPOLLIN; //监听读事件
    tep.data.fd = lfd; //监听lfd是否有客户端连接请求
    int ret = epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &tep); //将lfd及附带数据挂载到红黑树，即监听lfd
    if(ret == -1)
    {
        perror("epoll ctl error!");
        exit(1); //非零值：程序异常退出
    }

    int nready = 0; //接收epoll_wait函数的返回值，已经就绪的fd个数

    while(1)
    {
        //服务器循环阻塞监听
        nready = epoll_wait(efd, ep, 1024, -1); //填充就绪fd及连带数据的 数组名称、大小，阻塞监听
        if(nready == -1)
        {
            if(errno == EINTR)
            {
                continue; // 被信号打断，继续等待
            }
            perror("epoll wait error!");
            exit(1); 
        }

        for(int i = 0; i < nready; i++)
        {
            //如果是lfd监听事件，代表有新的客户端连接
            if(ep[i].data.fd == lfd)
            {
                //4.阻塞等待客户端连接
                clit_addr_len = sizeof(clit_addr);
                cfd = Accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len); //能与服务器进行数据通信的socket对应的文件描述符
                //打印客户端信息
                printf("client IP : %s, port : %d\n", inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(clit_addr.sin_port));

                //把该客户端的cfd通信套接字的描述符加入监听中
                tep.events = EPOLLIN; //监听读事件
                tep.data.fd = cfd; 
                ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &tep); //加入红黑树，监听cfd是否有读写请求
                if(ret == -1)
                {
                    perror("epoll ctl error!");
                    exit(1); 
                }
            }
            else //不是lfd代表是cfd通信事件
            {
                int sockfd = ep[i].data.fd; //接收发生事件的文件描述符
                int n = Read(sockfd, buf, sizeof(buf)); //ret保存读取到的字节数，一次调用能读多少读多少，不会超过buf的大小
                //⭐ 读到0说明客户端关闭连接
                if(n <= 0) 
                {
                    if(n < 0) //出错处理
                    {
                        perror("read error!");
                    }
                    //剩下的代码[出错]和[客户端关闭连接]处理逻辑一致
                    ret = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL); //将该描述符从监听集合中移除，删除不需要传入结构体
                    if(ret == -1)
                    {
                        perror("epoll ctl error!");
                        exit(1); 
                    }
                    Close(sockfd);
                }
                else
                {
                    Write(STDOUT_FILENO, buf, n);  //在终端上显示收到的数据
                    for(int j = 0; j < n; j++)
                    {
                        buf[j] = toupper(buf[j]); //逐个字符进行转换
                    }
                    Write(sockfd, buf, n); //向文件描述符为sockfds的文件，写入同样字节大小的数据
                }
            }
        }
    }
    return 0;
}