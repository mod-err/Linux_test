#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h> //toupper
#include "wrap.h"

#define SERV_PORT 9527

int main(int argc, char* argv[])
{
    int ret;

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

    //4.阻塞等待客户端连接
    clit_addr_len = sizeof(clit_addr);
    cfd = Accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len); //能与服务器进行数据通信的socket对应的文件描述符

    printf("client IP : %s, port : %d\n", inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(clit_addr.sin_port));

    //5.读写数据
    while(1)
    {
        ret = Read(cfd, buf, sizeof(buf)); //ret保存读取到的字节数，一次调用能读多少读多少，不会超过buf的大小
        Write(STDOUT_FILENO, buf, ret);  //在终端上显示

        for(int i = 0; i < ret; i++)
        {
            buf[i] = toupper(buf[i]); //逐个字符进行转换
        }

        Write(cfd, buf, ret); //向文件描述符为cfd的文件，写入同样字节大小的数据
    }

    //6.关闭
    Close(lfd);
    Close(cfd);

    return 0;
}