#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>

using namespace std;

int main()
{
    //在磁盘上创建待映射到内存的文件
    int fd = open("testmap", O_RDWR | O_CREAT, 0644); //-rw-r--r--
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    //指定文件大小为20字节
    lseek(fd, 19, SEEK_END);
    write(fd, "\0", 1);

    //获取文件大小
    int len = lseek(fd, 0, SEEK_END);

    //创建共享内存映射
    char* p = NULL;
    p = (char*)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //c++中必须强制显示把void*转为char*
    if(p == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }

    //把字符串拷贝到p中，写文件操作
    strcpy(p, "hello mmap");
    //打印，读文件操作
    cout << p << endl; //使用cout打印char* 类指针p会直接输出内容；其他类型指针会输出指针的地址

    //回收映射的内存
    int ret = munmap(p, len);
    if(ret == -1)
    {
        perror("mmap error");
        exit(1); 
    }

    return 0;
}