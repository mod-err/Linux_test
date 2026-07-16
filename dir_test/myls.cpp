#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>


using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	DIR* dp;
	dp = opendir(argv[1]);
	if(dp == NULL)
	{
		perror("opendir error");
		exit(1);
	}	
	
	struct dirent* sdp;
	while((sdp = readdir(dp)) != NULL)
	{
		if (strcmp(sdp->d_name, ".") == 0 || strcmp(sdp->d_name, "..") == 0) {
		    continue;
		}
		cout << sdp->d_name << endl;
	}

	closedir(dp);

	return 0;
}
