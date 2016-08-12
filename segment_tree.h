#pragma once;
#include <vector>
#include <algorithm>
#include <fstream>
using std::vector;
typedef unsigned int uint;

class segment_tree {
private:
	struct node {
		uint left, right;
		int value;
		int inconsistency;

		node(int value, uint l, uint r, int inconsistency = 0);
	};

	vector<node> data;
	uint size;
	uint capacity;

	// least power of 2 greater or equal than n
	uint lpo2(uint n);

	int _get(uint index, uint l, uint r);

public:
	segment_tree();
	segment_tree(vector<int>& input);

	int get(uint l, uint r);
	int get(uint index);
};