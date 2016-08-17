#include "segment_tree.h"

segment_tree::node::node(int value, uint l, uint r, int inconsistency, bool is_filled) : value(value), left(l), right(r), inconsistency(inconsistency), filled(is_filled) {};

int segment_tree::node::get() {
	return (filled ? 0 : value) + inconsistency * (right - left);
}

uint segment_tree::lpo2(uint n) {
	--n;
	for (int i = 1; i < 32; i *= 2)
		n |= n >> i;
	return ++n;
}

void segment_tree::_check_range(uint l, uint r) {
	if (r > size)
		throw std::out_of_range("r > size");
	if (l >= r)
		throw std::out_of_range("l >= r");
}

void segment_tree::_sift_down(uint index) {
	if (data[index].filled) {
		data[index * 2 + 1].filled = true;
		data[index * 2 + 2].filled = true;
		data[index].filled = false;
		data[index * 2 + 1].inconsistency = data[index].inconsistency;
		data[index * 2 + 2].inconsistency = data[index].inconsistency;
		data[index].inconsistency = 0;
	}
	else {
		data[index * 2 + 1].inconsistency += data[index].inconsistency;
		data[index * 2 + 2].inconsistency += data[index].inconsistency;
		data[index].inconsistency = 0;
	}
}

void segment_tree::_sift_up(uint index) {
	data[index].value = data[index * 2 + 1].get() + data[index * 2 + 2].get();
}

segment_tree::segment_tree() {
	size = 0;
	capacity = 0;
	data = vector<node>();
}

segment_tree::segment_tree(vector<int>& input) : segment_tree() {
	if (input.size()) {
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
}

int segment_tree::_get(uint index, uint l, uint r) {
	if (data[index].left >= r || data[index].right <= l)
		return 0;
	if (data[index].left >= l && data[index].right <= r)
		return data[index].get();
	_sift_down(index);
	int ans = _get(index * 2 + 1, l, r) + _get(index * 2 + 2, l, r);
	_sift_up(index);
	return ans;
}

int segment_tree::get(uint l, uint r) {
	_check_range(l, r);
	return _get(0, l, r);
}

int segment_tree::get(uint index) {
	return get(index, index + 1);
}

void segment_tree::_add(uint index, uint l, uint r, int summand) {
	if (data[index].left >= r || data[index].right <= l)
		return;
	if (data[index].left >= l && data[index].right <= r) {
		data[index].inconsistency += summand;
		return;
	}
	_sift_down(index);
	_add(index * 2 + 1, l, r, summand);
	_add(index * 2 + 2, l, r, summand);
	_sift_up(index);
}

void segment_tree::add(uint l, uint r, int summand) {
	_check_range(l, r);
	_add(0, l, r, summand);
}

void segment_tree::add(uint index, int summand) {
	add(index, index + 1, summand);
}

void segment_tree::_set(uint index, uint l, uint r, int value) {
	if (data[index].left >= r || data[index].right <= l)
		return;
	if (data[index].left >= l && data[index].right <= r) {
		data[index].filled = true;
		data[index].inconsistency = value;
		return;
	}
	_sift_down(index);
	_set(index * 2 + 1, l, r, value);
	_set(index * 2 + 2, l, r, value);
	_sift_up(index);
}

void segment_tree::set(uint l, uint r, int value) {
	_check_range(l, r);
	_set(0, l, r, value);
}

void segment_tree::set(uint index, int value) {
	set(index, index + 1, value);
}