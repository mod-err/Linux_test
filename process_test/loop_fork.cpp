#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int main()
{
    int n = 5; //创建n个子进程
    int i, status;
    pid_t pid, wpid, tpid;

    for(i = 0; i < n; i++)
    {
        pid = fork();
        
        if(pid == 0)
        {
           break;
        }

        //指定回收2th子进程
        if(i == 2)
        {
            cout << "pid = " << pid << endl;
        }
    }

    //父进程
    if(i == n)
    {
        sleep(n);  //休眠n秒保证父进程最后执行完毕
        cout << "parent process :" << " my ID : " << getpid() << endl;

        cout << "need waitpid : " << pid << endl;
        //wpid = waitpid(-1, &status, WNOHANG); //非阻塞回收任意一个子进程
        wpid = waitpid(pid, &status, WNOHANG);  //非阻塞回收指定子进程
        if(wpid == -1)
        {
            perror("waitpid error!");
            exit(1);
        }
        cout << "waitpid a child : " << wpid << " success!" << endl;
    }
    else
    {
        sleep(i);
        cout << i << "th" << " child process :" << " my ID : " << getpid() << endl;
    }

    return 0;
}