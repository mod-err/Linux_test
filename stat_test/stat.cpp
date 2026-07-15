#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>  //exit
#include <stdio.h>   //perror
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	struct stat buf;
	
	int ret = stat(argv[1], &buf);
	if(ret == -1)
	{	
		perror("stat error");
		exit(1);
	}
	
	if(S_ISREG(buf.st_mode))
	{
		cout << "It is a regular" << endl;
	} 
	else if(S_ISDIR(buf.st_mode))
	{		
		cout << "It is a dir" << endl;
	}
	//cout << "file size:" << buf.st_size << endl;
	return 0;
}
