#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main()
{
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid == 0)
    {
        execl("./myls", "./myls", "..", NULL);
        //失败之后才执行
        perror("execlp error");
        exit(1);
    }
    else if(pid > 0)
    {
        sleep(1);
        cout << "parent process : my ID : " << getpid() << " my child ID : " << pid << endl;
    }
}