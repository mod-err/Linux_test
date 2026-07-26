#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int val = 100;

int main(int argc, char* argv[])
{
    //创建共享映射区
    int fd = open("testmap", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }
    //指定文件大小
    lseek(fd, 4, SEEK_END);
    write(fd, "\0", 1);

    //获取文件大小
    int len = lseek(fd, 0, SEEK_END);

    //创建共享内存映射
    int* p;
    p = (int*)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }
    
    //创建父子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid == 0) //子进程
    {
        *p = 4000;
        val = 1000;
        cout << "child : *p = " << *p << " , val = " << val << endl;
    }
    else if(pid > 0) //父进程
    {
        sleep(1); //等待子进程写完
        
        cout << "parent : *p = " << *p << " , val = " << val << endl;
        wait(NULL); //回收子进程

        //回收映射的内存
        int ret = munmap(p, len);
        if(ret == -1)
        {
            perror("mmap error");
            exit(1); 
        }
    }
    
    return 0;
}