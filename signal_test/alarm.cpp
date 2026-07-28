#include <unistd.h>
#include <stdio.h>

int main()
{
    alarm(1);

    int i = 0;
    for(int i = 0; ;i++)
    {
        printf("%d\n", i);
    }
}