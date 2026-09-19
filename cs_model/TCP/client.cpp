#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define SERV_PORT 9527

int main(int argc, char* argv[])
{
    int cfd = 0, ret = 0;

    char buf[4096];

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); //主机字节序转为网络字节序
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr); //已知服务器的IP，转为二进制并传入IP

    //1.创建socket套接字
    cfd = socket(AF_INET, SOCK_STREAM, 0);

    //2.连接服务器，主动发起三次握手
    connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(cfd == -1)
    {
        perror("accept error!");
        exit(1);
    }

    //3.读写数据
    while(1)
    {
        write(cfd, "hello\n", 6);
        ret = read(cfd, buf, sizeof(buf)); //从cfd套接字文件中读取数据，存放在buf缓冲区中，不会超过buf的大小
        write(STDOUT_FILENO, buf, ret);  //读取到的数据显示到终端
        sleep(1);
    }

    //4.关闭
    close(cfd);

    return 0;
}