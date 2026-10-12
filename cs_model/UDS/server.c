#include <sys/un.h>    //UDS本地套接字
#include <sys/types.h> //socklen_t
#include <string.h>    //memset()、strcpy()
#include <sys/socket.h> //socket()、bind() ...
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>  //offsetof()
#include <ctype.h>   //toupper()

#define SERV_ADDR  "serv.socket"  

int main(int argc, char* argv[])
{
    int len, size, i; 
    int sfd, cfd; //本地套接字文件描述符
    struct sockaddr_un serv_addr, clit_addr;
    char buf[4096];
    socklen_t clit_addr_len; //接收客户端的协议大小

    //1.创建socket套接字
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);  //AF_UNIX本地进程间通信

    memset(&serv_addr, 0, sizeof(serv_addr)); //把网络协议壳清零
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SERV_ADDR); //字符串拷贝函数，把SERV_ADDR拷贝到serv_addr.sun_path中

    //⭐len = sun_family的大小[偏移量] + sun_path 实际字符串长度
    len = offsetof(struct sockaddr_un, sun_path) + strlen(serv_addr.sun_path);

    unlink(SERV_ADDR);  //⭐确保bind之前serv.sock文件不存在,bind会创建该文件

    //3.绑定地址和端口
    bind(sfd, (struct sockaddr*)&serv_addr, len);

    //4.监听
    listen(sfd, 128);

    printf("Accept ...\n"); 

    while(1)
    {
        clit_addr_len =  sizeof(clit_addr);  //单独起一个变量，因为其做为传入传出参数

        cfd = accept(sfd, (struct sockaddr *)&clit_addr, (socklen_t *)&clit_addr_len);

        clit_addr_len -= offsetof(struct sockaddr_un, sun_path);
        // 现在 clit_addr_len 就是 sun_path 的实际长度
        clit_addr.sun_path[clit_addr_len] = '\0';  //手动补上字符串结尾，确保打印时,没有乱码出现
        
        printf("client bind filename %s\n", clit_addr.sun_path); 

        while ((size = read(cfd, buf, sizeof(buf))) > 0) 
        {  
            for (i = 0; i < size; i++)
            {
                buf[i] = toupper(buf[i]); 
            }     
            write(cfd, buf, size);  
        } 
        close(cfd);
    }
    close(sfd);
    
    return 0;
}