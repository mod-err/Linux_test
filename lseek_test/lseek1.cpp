#include <unistd.h> //lseek
#include <stdio.h>  //perror
#include <stdlib.h> //exit
#include <unistd.h> //close
#include <fcntl.h>  //open
#include <string.h> //strlen

int main()
{
	char msg[] = "It is a test for lseek!\n";
	char ch;	
	int n;

	int fd = open("lseek.txt", O_RDWR | O_CREAT, 0644); //-rwx-r-x-r-x
	if(fd < 0)
	{
		perror("open lseek.txt error");
		exit(1);
	}

	write(fd, msg, strlen(msg));
	
	//lseek(fd, 0, SEEK_SET);
	
	while((n = read(fd, &ch, 1)))
	{
		if(n < 0)
		{
			perror("read error");
			exit(1);
		}
		write(STDOUT_FILENO, &ch, n);
	}
	
	close(fd);

	return 0;
}
