#pragma once;
#include <vector>
#include <algorithm>
#include <exception>

using std::vector;
typedef unsigned int uint;

class segment_tree {
private:
	struct node {
		uint left, right;
		int value;
		int inconsistency;
		bool filled;
		int get();

		node(int value, uint l, uint r, int inconsistency = 0, bool is_filled = 0);
	};

	vector<node> data;
	uint size;
	uint capacity;

	// least power of 2 greater or equal than n
	uint lpo2(uint n);

	// boundaries
	void _check_range(uint l, uint r);

	// propagate inconsistency down
	void _sift_down(uint index);

	// accumulate answer from children
	void _sift_up(uint index);

	int _get(uint index, uint l, uint r);
	void _add(uint index, uint l, uint r, int summand);
	void _set(uint index, uint l, uint r, int value);

public:
	segment_tree();
	segment_tree(vector<int>& input);

	// get answer on segment
	int get(uint l, uint r);
	// get element
	int get(uint index);

	// add constant value to each element in segment
	void add(uint l, uint r, int summand);
	// add value to given element
	void add(uint index, int summand);

	// set all elements in segment to value
	void set(uint l, uint r, int value);
	// set element to value
	void set(uint index, int value);
};