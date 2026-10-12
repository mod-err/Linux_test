#include <sys/un.h>    //UDS本地套接字
#include <sys/types.h> //socklen_t
#include <sys/socket.h> //socket()、bind() ...
#include <stddef.h>     //offsetof()
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>   //memset()

#define SERV_ADDR "serv.socket"
#define CLIE_ADDR "clie.socket" 

int main(int argc, char* argv[])
{
    int cfd, len;
    char buf[4096];
    struct sockaddr_un serv_addr, clit_addr;

    //构造客户端地址
    memset(&clit_addr, 0, sizeof(clit_addr)); 
    clit_addr.sun_family = AF_UNIX;
    strcpy(clit_addr.sun_path, CLIE_ADDR);

    //1.创建socket套接字
    cfd = socket(AF_UNIX, SOCK_STREAM, 0);

    //2.计算客户端地址结构有效长度
    len = offsetof(struct sockaddr_un, sun_path) + strlen(clit_addr.sun_path);

    unlink(CLIE_ADDR);  //⭐确保bind之前CLIE_ADDR文件不存在,bind会创建该文件

    //3.绑定地址和端口
    bind(cfd, (struct sockaddr*)&clit_addr, len);

    //构造服务器地址
    memset(&serv_addr, 0, sizeof(serv_addr)); //把网络协议壳清零
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SERV_ADDR);

    len = offsetof(struct sockaddr_un, sun_path) + strlen(serv_addr.sun_path);

    //4.与服务器建立连接
    connect(cfd, (struct sockaddr *)&serv_addr, len);

    //5.读写数据：键盘输入数据
    while (fgets(buf, 4096, stdin) != NULL)
    {
        //向服务器的写入数据
        write(cfd, buf, strlen(buf));  //strlen(buf)有效数据的长度
        //从cfd套接字文件中读取处理好的数据
        len = read(cfd, buf, sizeof(buf)); //sizeof(buf) == 4096;
        write(STDOUT_FILENO, buf, len);  //读取到的数据显示到终端
    }

    //6.关闭
    close(cfd);

    return 0;
}