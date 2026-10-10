#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>   //memset()

#define SERV_PORT 9527

int main(int argc, char* argv[])
{
    int cfd = 0, ret = 0;

    char buf[4096];

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); //把网络协议壳清零

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); //主机字节序转为网络字节序
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr); //已知服务器的IP，转为二进制并传入IP

    //1.创建socket套接字
    cfd = socket(AF_INET, SOCK_DGRAM, 0);

    //3.读写数据：键盘输入数据
    while (fgets(buf, 4096, stdin) != NULL)
    {
        //向服务器的 "IP+端口" 写入数据
        ret = sendto(cfd, buf, strlen(buf), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if(ret < 0)
        {
            perror("sendto error"); 
        } 
        ret = recvfrom(cfd, buf, 4096, 0, NULL, 0);  //NULL:不关心对端信息  
        if(ret < 0)
        {
            perror("recvfrom error"); 
        } 
        write(STDOUT_FILENO, buf, ret);  //读取到的数据显示到终端
    }

    //4.关闭
    close(cfd);

    return 0;
}