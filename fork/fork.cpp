#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

int main()
{
    cout << "before fork 1" << endl;
    cout << "before fork 2" << endl;
    cout << "before fork 3" << endl;
    cout << "before fork 4" << endl;

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
    }
    else if(pid == 0)
    {
        cout << "child process : child is creat!" << " my ID : " << getpid() << " my parent ID : " << getppid() << endl;
    }
    else if(pid > 0)
    {
        cout << "parent process : my child is " << pid << " my ID : " << getpid() << " my parent ID : " << getppid() << endl;
    }
    cout << "end of file!" << endl;

    return 0;
}