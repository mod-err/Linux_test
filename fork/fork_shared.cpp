#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

int main()
{
    int val = 100;

    pid_t pid = fork();

    if(pid == 0)
    {
        //val = 200;
        cout << val << endl;
        cout << "child process : my ID : " << getpid() << " my parent ID : " << getppid() << endl;
    }
    else if(pid > 0)
    {
        val = 300;
        cout << val << endl;
        cout << "parent process : my child ID " << pid << " my ID : " << getpid() << endl;
    }

    return 0;
}