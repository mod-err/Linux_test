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
    pid_t pid, wpid;

    for(i = 0; i < n; i++)
    {
        pid = fork();
        
        if(pid == 0)
        {
           break;
        }
    }

    //父进程
    if(i == n)
    {
        cout << "parent process :" << " my ID : " << getpid() << endl;

        //阻塞回收所有子进程
        // while((wpid = waitpid(-1, &status, 0)) != -1)
        // {
        //     cout << "waitpid a child : " << wpid << " success!" << endl;
        // }
        //非阻塞回收所有子进程
        while((wpid = waitpid(-1, &status, WNOHANG)) != -1)
        {
            if(wpid > 0)
            {
                cout << "waitpid a child : " << wpid << " success!" << endl;
            }
        }
        cout << "waitpid all process!" << endl;
    }
    else
    {
        sleep(i);
        cout << i << "th" << " child process :" << " my ID : " << getpid() << endl;
    }

    return 0;
}