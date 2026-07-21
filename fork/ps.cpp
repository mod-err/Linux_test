#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main()
{
    int fd = open("ps.txt", O_RDWR | O_CREAT, 0644);
    //标准输出STDOUT_FILENO，即终端指向fd
    dup2(fd, STDOUT_FILENO);

    pid_t pid = fork();
    if(pid == 0)
    {
        execlp("ps", "ps", "aux", NULL);
        perror("execlp error");
        exit(1);
    }
    else if(pid > 0)
    {
        sleep(1);
        cout << "parent process : my ID : " << getpid() << " my child ID : " << pid << endl;
    }
}