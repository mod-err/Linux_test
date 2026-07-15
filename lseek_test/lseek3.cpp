#include <unistd.h> //lseek
#include <stdio.h>  //perror
#include <stdlib.h> //exit
#include <unistd.h> //close
#include <fcntl.h>  //open
#include <string.h> //strlen
#include <iostream> //cout

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	int fd = open(argv[1], O_RDWR); 
	if(fd < 0)
	{
		perror("open error");
		exit(1);
	}

	int length = lseek(fd, 75, SEEK_END);	
	cout << "file size: " << length << endl;
	
	write(fd, "X", 1);

	close(fd);

	return 0;
}
