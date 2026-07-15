#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char buf[1024];	
	int n = 0;

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1)
	{
		perror("open source file error");
		exit(1);
	}
	int fd2 = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, 0644);

	while((n = read(fd1, buf, 1024)) != 0)
	{
		if(n == -1)
		{
			perror("read error");
			break;
		}
		write(fd2, buf, n);
	}

	close(fd1);
	close(fd2);

	return 0;
}
