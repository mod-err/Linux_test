#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    pid_t pid = fork();
    //子进程
    if(pid == 0)
    {
        //创建会话
        setsid();
        //改变工作目录
        chdir("~/Desktop/test");
        //改变文件访问权限掩码
        umask(0022); //文件最大权限666，减去022，得到文件实际权限644，-rw-r--r--
        //关闭文件描述符0-1-2
        close(STDIN_FILENO); //文件描述符0
        int fd = open("/dev/null", O_RDWR); //文件描述符0被释放，fd使用最小的空闲描述符fd=0
        dup2(fd, STDOUT_FILENO);//文件描述符1，重定向到文件描述符0
        dup2(fd, STDERR_FILENO);//文件描述符2，重定向到文件描述符0

        while(1);
    }

    return 0;
}