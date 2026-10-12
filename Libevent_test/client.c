#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <string.h>  
#include <fcntl.h>  
#include <event2/event.h>  
  
//对操作处理函数（回调函数必须接收三个参数：触发事件的文件描述符、实际触发的事件类型、用户自定义参数）
void write_callback(evutil_socket_t fd, short what, void *arg)
{  
    //write管道  
    char buf[1024];  
      
    static int num = 0;  
    sprintf(buf, "hello,world-%d\n", num++);  
    write(fd, buf, strlen(buf)+1);  
      
    sleep(1);  
}  
 
// 写管道  
int main(int argc, const char* argv[])  
{  

    int fd = open("myfifo", O_WRONLY);  	
    if(fd == -1)  
    {  
        perror("open error");  
        exit(1);  
    }  
    
    //创建事件底座 
    struct event_base* base = event_base_new();  
    //创建事件
    struct event* ev = event_new(base, fd, EV_WRITE | EV_PERSIST, write_callback, NULL);  
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