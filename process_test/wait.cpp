#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0) {
        printf("---child, my parent= %d, going to sleep 10s\n", getppid());
        sleep(10);
        printf("---child die---\n");
    } else if (pid > 0) {
        wpid = wait(&status);
        if(status == -1)
        {
            perror("wait error");
            exit(1);
        }
        printf("parent wait finish, recycled pid: %d\n", wpid);
    } else {
        perror("fork");
        return 1;
    }
}