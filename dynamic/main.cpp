#include <iostream>
#include "mymath.h"
 
using namespace std;

int main()
{
	int a = 10;
	int b = 2;
	cout << a << "+" << b << "=" << add(a, b) << endl;
	cout << a << "-" << b << "=" << sub(a, b) << endl;
	cout << a << "*" << b << "=" << mul(a, b) << endl;
	return 0;
}
