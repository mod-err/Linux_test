#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

int main()
{
    int fd, ret;
    const char *p = "test of unlink\n";

    //创建并打开临时文件
    fd = open("temp.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd < 0){
        perror("open temp error");
        exit(1);
    }
    //unlink后依然可以正常写入文件
    ret = write(fd, p, strlen(p));
    if(ret == -1){
        perror("write error");
    }
    cout << "write success!" << endl;
    getchar();
    
    //立即删除目录项，进程仍可正常读写
    ret = unlink("temp.txt");
    if(ret < 0){
        perror("unlink error");
        exit(1);
    }
    
    close(fd);
    return 0;
}
