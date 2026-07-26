#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>

using namespace std;

struct student
{
    int id;
    char name[20];
    int age;
};

int main(int argc, char* argv[])
{
    struct student stu = {0, "zhangsan", 18};

    //创建共享映射区
    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }
    //指定文件大小
    lseek(fd, 19, SEEK_END);
    write(fd, "\0", 1);

    //获取文件大小
    int len = lseek(fd, 0, SEEK_END);

    //创建共享内存映射
    struct student* p;
    p = (struct student*)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }

    //从共享内存映射读
    while(1)
    {
        cout << "id : " << p->id  << " name : " << p->name << " age : " << p->age << endl;
        sleep(1);
    }
    
    
    //回收映射的内存
    int ret = munmap(p, len);
    if(ret == -1)
    {
        perror("mmap error");
        exit(1); 
    }
    
    return 0;
}