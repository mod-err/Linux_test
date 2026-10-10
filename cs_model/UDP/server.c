#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>   //memset()
#include <ctype.h>    //toupper

#define SERV_PORT 9527

int main(int argc, char* argv[])
{
    int ret;

    int lfd = 0;

    char buf[4096], client_IP[1024]; //接收客户端点分十进制IP地址

    struct sockaddr_in serv_addr, clit_addr;
    socklen_t clit_addr_len;

    memset(&serv_addr, 0, sizeof(serv_addr)); //把网络协议壳清零
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); //主机字节序转为网络字节序
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //本机IP，同样转为网络字节序
    
    //1.创建socket套接字
    lfd = socket(AF_INET, SOCK_DGRAM, 0);  //报式协议

    //2.绑定端口和IP
    bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Accepting connections ...\n"); 

    //5.读写数据
    while(1)
    {
        clit_addr_len = sizeof(clit_addr);
        ret = recvfrom(lfd, buf, 4096, 0, (struct sockaddr *)&clit_addr, &clit_addr_len);
        if(ret < 0)
        {
            perror("recvfrom error"); 
        } 

        printf("client IP : %s, port : %d\n", inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(clit_addr.sin_port));

        //write(STDOUT_FILENO, buf, ret);  //在终端上显示

        for(int i = 0; i < ret; i++)
        {
            buf[i] = toupper(buf[i]); //逐个字符进行转换
        }

        ret = sendto(lfd, buf, ret, 0, (struct sockaddr *)&clit_addr, clit_addr_len);
        if(ret < 0)
        {
            perror("sendto error"); 
        } 
    }

    //6.关闭
    close(lfd);

    return 0;
}