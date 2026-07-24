#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

int main(int argc, char* argv[])
{
    //以只写方式打开管道
    int fd = open(argv[1], O_WRONLY);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    //持续向管道写数据
    int i = 0;
    char buf[4096]; //缓冲区
    while(1)
    {
        sprintf(buf, "hello fifo %d\n", i++);
        write(fd, buf, strlen(buf)); //向管道写数据
        sleep(1);
    }

    close(fd);
}