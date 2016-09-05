#pragma once
#include <vector>
using std::vector;

class dsu {
private:	
	vector<int> p;

public:
	dsu(int n);

	int add();

	int find(int a);

	void unite(int a, int b);
};