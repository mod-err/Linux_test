#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <cstring>


using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    int fd1 = open(argv[1], O_RDWR | O_CREAT, 0644);
    int fd2 = open(argv[2], O_RDWR | O_CREAT, 0644);

    int newfd = dup2(fd1, fd2); //fd2指向fd1

    const char* buf = "123456789\n";
    int ret = write(newfd, buf, strlen(buf));

    cout << "fd1 = " << fd1 << endl;
    cout << "fd2 = " << fd2 << endl;
    cout << "newfd = " << newfd << endl;
    cout << "写入" << ret << "字节" << endl;

    return 0;
}