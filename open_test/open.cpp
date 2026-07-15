#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>

using namespace std;

int main()
{
	int fd;
	
	fd = open("test.txt", O_RDONLY);
	fd = open("test1.txt", O_RDONLY | O_CREAT, 0644);
	fd = open("test2.txt", O_TRUNC);
	fd = open("test3.txt", O_RDONLY | O_CREAT, 0511);	
	
	//cout << fd << endl;	

	//fd = open("test3.txt", O_RDONLY);
	//fd = open("test3.txt", O_WRONLY);	
	//fd = open("mydir", O_WRONLY);	

	cout << "fd=" <<  fd << " " << "errno=" << errno << ":" << strerror(errno) << endl;
	
	close(fd);
}
