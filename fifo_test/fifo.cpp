#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int ret = mkfifo("myfifo1", 0644);
    if(ret == -1)
    {
        perror("mkfifo error");
        exit(1);
    }
    return 0;
}