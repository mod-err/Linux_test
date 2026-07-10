#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

void init(vector<int>& vec, int size)
{
	for(int i = 0; i < size; i++)
	{
		vec.push_back(rand()%100); //0-99
	}
}

void show(vector<int>& vec)
{
	for(int i = 0; i < vec.size(); i++)
	{
		cout << "vec" << "[" << i << "]" << "=" << vec[i] << " ";
	}
	cout << endl;
}

int paration(vector<int>& vec, int l, int r)
{
	int val = vec[l];
	while(l < r)
	{
		while(l < r && vec[r] > val)
		{
			r--;
		}
		if(l < r)
		{
			vec[l] = vec[r];
			l++;
		}
		while(l < r && vec[l] < val)
		{
			l++;
		}
		if(l < r)
		{
			vec[r] = vec[l];
			r--;
		}
	}
	vec[l] = val;
	return l;
}

void quick_sort(vector<int>& vec, int begin, int end)
{
	if(begin >= end)
	{
		return;
	}

	int pos = paration(vec, begin, end);

	quick_sort(vec, begin, pos - 1);
	quick_sort(vec, pos + 1, end);
}

int main()
{
	srand(time(NULL));
	
	vector<int> vec;
	init(vec, 10);
	show(vec);
	quick_sort(vec, 0, 9);
	show(vec);
}
