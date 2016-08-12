#include "segment_tree.h"


segment_tree::node::node(int value, uint l, uint r, int inconsistency) : value(value), left(l), right(r), inconsistency(inconsistency) {};

uint segment_tree::lpo2(uint n) {
	--n;
	for (int i = 1; i < 32; i *= 2)
		n |= n >> i;
	return ++n;
}

int segment_tree::_get(uint index, uint l, uint r) {
	if (data[index].left >= r || data[index].right <= l)
		return 0;
	if (data[index].left >= l && data[index].right <= r)
		return data[index].value;
	return _get(index * 2 + 1, l, r) + _get(index * 2 + 2, l, r);
}

segment_tree::segment_tree() {
	size = 0;
	capacity = 0;
	data = vector<node>();
}

segment_tree::segment_tree(vector<int>& input) {
	size = input.size();
	capacity = lpo2(size);
	data = vector<node>(capacity * 2 - 1, node(0, 0, 0));
	for (int i = 0; i < capacity; ++i) {
		if (i < size)
			data[i + capacity - 1] = node(input[i], i, i + 1);
		else {
			data[i + capacity - 1] = node(0, i, i + 1);
		}
	}
	for (int i = capacity - 2; i >= 0; --i) {
		data[i] = node(data[i * 2 + 1].value + data[i * 2 + 2].value, data[i * 2 + 1].left, data[i * 2 + 2].right);
	}
}

int segment_tree::get(uint l, uint r) {
	r = std::min(r, size);
	l = std::min(l, r);
	return _get(0, l, r);
}

int segment_tree::get(uint index) {
	return get(index, index + 1);
}
