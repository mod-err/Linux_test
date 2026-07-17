#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>

using std::cout;
using std::endl;

void isFile(const char* name);

void read_dir(const char* dir)
{
    char path[256];

    DIR* dp;
    dp = opendir(dir);

    if(dp == NULL)
    {
        perror("opendir error");
        exit(1);
    }

    struct dirent* sdp;
    while((sdp = readdir(dp)) != NULL)
    {
        if(strcmp(sdp->d_name, ".") == 0 || strcmp(sdp->d_name, "..") == 0)
        {
            continue;
        }
        //拼接目录项和其路径
        sprintf(path, "%s/%s", dir, sdp->d_name);
        //递归
        isFile(path);
    }

    closedir(dp);
}

void isFile(const char* name)
{
    struct stat sb;

    //stat需要完整路径才可以判断文件，不能只传入文件名称，所以要拼接
    int ret = stat(name, &sb);
    if(ret == -1)
    {
        perror("stat error");
		exit(1);
    }

    //是目录文件，递归遍历
    if(S_ISDIR(sb.st_mode))
    {
        read_dir(name);
    }

    //普通文件，打印其名称和大小
    cout << name << "\t" << sb.st_size << endl;
}

int main(int argc, char* argv[])
{
    //如果用户未传入，默认访问当前路径
    if(argc == 1)
    {
        isFile(".");
    }
    else
    {
        isFile(argv[1]);
    }

    return 0;
}