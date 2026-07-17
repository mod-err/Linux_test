#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    int fd = open(argv[1], O_RDWR | O_CREAT, 0644); //-rw-r--r--
    cout << "fd = " << fd << endl;

    int newfd = dup(fd);
    cout << "newfd = " << newfd << endl;
    
    const char* buf = "123456\n";
    int ret = write(newfd, buf, strlen(buf));
    cout << "写入" << ret << "字节" << endl;

    return 0;
}