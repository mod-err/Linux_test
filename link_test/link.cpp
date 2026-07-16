#include <unistd.h>

int main(int argc, char* argv[])
{
	link(argv[1], argv[2]);
	unlink(argv[1]);
	
	return 0;
}
