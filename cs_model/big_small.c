#include <stdio.h>

int main()
{
	short a = 1;
	int b = *(char*)(&a);       //见下详解               
	if (b == 0)
	{
		printf("大端！\n");
	}
	else
		printf("小端！\n");
	return 0;
}