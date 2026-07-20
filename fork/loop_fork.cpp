#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

int main()
{
    int i;
    pid_t pid;

    for(i = 0; i < 3; i++)
    {
        pid = fork();
        
        if(pid == 0)
        {
           break;
        }
    }

    if(i == 3)
    {
        sleep(3);
        cout << "parent process :" << " my ID : " << getpid() << endl;
    }
    else
    {
        sleep(i);
        cout << i << "th" << " child process :" << " my ID : " << getpid() << endl;
    }

    return 0;
}