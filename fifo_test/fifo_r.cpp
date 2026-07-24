#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    //以只读方式打开管道
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    //持续从管道读数据
    int i = 0;
    char buf[4096]; //缓冲区
    int len; //读取到的数据长度
    while(1)
    {
        len = read(fd, buf, sizeof(buf)); //从管道读数据
        write(STDOUT_FILENO, buf, len);   //写出到终端
    }

    close(fd);
}
