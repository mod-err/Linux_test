#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>

using std::cout;
using std::endl;

void read_dir(char* argv)
{
    DIR* dp;
    dp = opendir(argv);

    if(dp == NULL)
    {
        perror("opendir error");
        exit(1);
    }

    struct dirent* sdp;
    while((sdp = readdir(dp)) != NULL)
    {
        if(strcmp(sdp->d_name, ".") || strcmp(sdp->d_name, ".."))
        {
            continue;
        }
        cout << sdp->d_name << endl;

        //递归
        read_dir(sdp->d_name);
    }

    closedir(dp);
}

void isFile(char* name)
{
    struct stat sb;

    int ret = stat(name, &sb);
    if(ret == -1)
    {
        perror("stat error");
		exit(1);
    }

    //是目录文件
    if(S_ISDIR(sb.st_mode))
    {
        read_dir(name);
    }

    //普通文件，打印其名称和大小
    cout << name << "\t" << sb.st_size << endl;
}

int main(int argc, char* argv[])
{
    isFile(argv[1]);

    return 0;
}