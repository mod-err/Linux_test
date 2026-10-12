#include <stdio.h>
#include <unistd.h>  
#include <stdlib.h> 
#include <string.h>  
#include <fcntl.h> 
#include <event2/event.h>
#include <sys/types.h> 
#include <sys/stat.h>

//对操作处理函数（回调函数必须接收三个参数：触发事件的文件描述符、实际触发的事件类型、用户自定义参数）
void read_callback(evutil_socket_t fd, short what, void *arg)
{
    char buf[1024];
    int len = read(fd, buf, sizeof(buf));  
    //打印读取的内容
    printf("data len = %d, buf = %s\n", len, buf); 
    sleep(1);
}

int main(int argc, char* argv[])
{
    //创建有名管道  
    mkfifo("myfifo", 0664); 
    int fd = open("myfifo", O_RDONLY); 
    if(fd == -1)  
    {  
        perror("open error");  
        exit(1);  
    } 
    
    //创建事件底座
    struct event_base *base = event_base_new();
    //创建事件
    struct event * ev = event_new(base, fd, EV_READ | EV_PERSIST, read_callback, NULL); //用户无自定义参数所以最后一个参数传入NULL
    //将事件添加到底座
    event_add(ev, NULL);
    //启动事件循环
    event_base_dispatch(base);
     // 释放资源  
    event_free(ev);  
    event_base_free(base);  
    close(fd);  
         
    return 0; 
}