#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h> //toupper
#include "wrap.h"

#define SERV_PORT 9527

int main(int argc, char* argv[])
{
    int ret, nready;

    int lfd = 0, cfd = 0; //保存socket文件的文件描述符

    char buf[4096], client_IP[1024]; //接收客户端点分十进制IP地址

    struct sockaddr_in serv_addr, clit_addr;
    socklen_t clit_addr_len;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); //主机字节序转为网络字节序
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //本机IP，同样转为网络字节序
    
    //1.创建socket套接字
    lfd = Socket(AF_INET, SOCK_STREAM, 0); 

    //2.绑定端口和IP
    Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //3.设置与服务器同时连接的客户端上限
    Listen(lfd, 128);

    /*------------保存最大文件描述符----------------*/
    int maxfd = lfd;
    fd_set rset, allset; //allset设置读监听的集合，rset做为allset的备份
    FD_ZERO(&allset); //清空集合
    FD_SET(lfd, &allset); //将lfd加入到监听集合

    while(1)
    {
        //fd_set在select是传入传出参数，allset做为传入参数为想监听的文件描述符，rset为传出参数为实际有信号的文件描述符
        rset = allset;
        //监听文件描述符对应集合
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if(nready < 0)
        {
            perror("select error!");
            exit(1);
        }
        //如果lfd在传出的rset中表示有客户端要进行连接
        if(FD_ISSET(lfd, &rset))
        {
           //4.阻塞等待客户端连接
            clit_addr_len = sizeof(clit_addr);
            cfd = Accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len); //能与服务器进行数据通信的socket对应的文件描述符
            printf("client IP : %s, port : %d\n", inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(clit_addr.sin_port));
            //向监听集合加入cfd，监听cfd是否有读事件
            FD_SET(cfd, &allset);
            //更新最大的文件描述符
            if(maxfd < cfd)
            {
                maxfd = cfd;
            }
            //nready=1说明只有lfd事件，没有通信事件，直接开始新一轮循环监听
            if(nready == 1)
            {
                continue;
            }
        }
        //开始进行通信即数据处理，先检测哪个clients有数据就绪
        for(int i = lfd + 1; i <= maxfd; i++)
        {
            //FD_ISSET(i, &rset) == 1即i对应的client有数据就绪
            if(FD_ISSET(i, &rset))
            {
                //开始进行数据处理
                ret = Read(i, buf, sizeof(buf)); //ret保存读取到的字节数，一次调用能读多少读多少，不会超过buf的大小
                //如果ret = 0就说明客户端关闭了连接, ⭐ 统一处理断开和错误
                if(ret <= 0)
                {
                    close(i);
                    FD_CLR(i, &allset); //将该描述符从监听集合中移除
                    continue;
                }
                //实际读到了字节，进行数据处理
                Write(STDOUT_FILENO, buf, ret);  //在终端上显示收到的数据

                for(int i = 0; i < ret; i++)
                {
                    buf[i] = toupper(buf[i]); //逐个字符进行转换
                }

                Write(i, buf, ret); //向文件描述符为cfd的文件，写入同样字节大小的数据
            }
        }
    }
    //6.关闭
    Close(lfd);

    return 0;
}