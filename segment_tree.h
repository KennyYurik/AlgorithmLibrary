#pragma once;
#include <vector>
#include <algorithm>
#include <exception>
#include "monoid.h"

using std::vector;
typedef unsigned int uint;

/*
	segment_tree<some_monoid<type>>
   
	Monoid - struct with zero, append and pow (look at monoid.h)

	segment_tree methods:
	size() - returns elements count, O(1)
	empty() - is tree empty?, O(1)
	get() - returns answer of append() on segment or just element, O(log)
	set() - sets all elements on segment or just one element to some value, O(log)
	add() - append value to all elements on segment, O(log)
	push_back() - add element to end of tree, O(1) amortized
	pop_back() - remove element from end of tree, O(1) amortized

*/

template<typename Monoid>
class segment_tree {
private:
	typedef Monoid m;
	typedef typename m::type T;

	struct node {
		uint left, right;
		T value;
		T inconsistency;
		bool filled;
		T get();

		node(T value, uint l, uint r, T inconsistency = m::zero, bool is_filled = 0);
	};

	vector<node> data;
	int _size;
	int _capacity;

	// least power of 2 greater or equal than n
	uint lpo2(uint n);

	// boundaries
	void _check_range(uint l, uint r);

	// propagate inconsistency down
	void _sift_down(uint index);

	// accumulate answer from children
	void _sift_up(uint index);

	// get rid of all laziness
	void _sift_all();

	T _get(uint index, uint l, uint r);
	void _add(uint index, uint l, uint r, T summand);
	void _set(uint index, uint l, uint r, T value);

	// change the capacity
	void _resize(uint new_cap);

public:
	segment_tree();
	segment_tree(vector<T>& input);

	// elements count
	uint size();
	bool empty();

	// get answer on segment
	T get(uint l, uint r);
	// get element
	T get(uint index);

	// add constant value to each element in segment
	void add(uint l, uint r, T summand);
	// add value to given element
	void add(uint index, T summand);

	// set all elements in segment to value
	void set(uint l, uint r, T value);
	// set element to value
	void set(uint index, T value);

	// as in vector
	void push_back(T elem);
	void pop_back();
};

template<typename Monoid>
inline segment_tree<Monoid>::node::node(T value, uint l, uint r, T inconsistency, bool is_filled) : value(value), left(l), right(r), inconsistency(inconsistency), filled(is_filled) {};

template<typename Monoid>
inline typename Monoid::type segment_tree<Monoid>::node::get() {
	return m::append(filled ? m::zero : value, m::pow(inconsistency, (right - left)));
}

template<typename Monoid>
uint segment_tree<Monoid>::lpo2(uint n) {
	--n;
	for (int i = 1; i < 32; i *= 2)
		n |= n >> i;
	++n;
	if (n == 0)
		++n;
	return n;
}

template<typename Monoid>
void segment_tree<Monoid>::_check_range(uint l, uint r) {
	if (r > (uint)_size)
		throw std::out_of_range("r > _size");
	if (l >= r)
		throw std::out_of_range("l >= r");
}

template<typename Monoid>
void segment_tree<Monoid>::_sift_down(uint index) {
	if (data[index].filled) {
		data[index * 2 + 1].filled = true;
		data[index * 2 + 2].filled = true;
		data[index].filled = false;
		data[index * 2 + 1].inconsistency = data[index].inconsistency;
		data[index * 2 + 2].inconsistency = data[index].inconsistency;
		data[index].inconsistency = m::zero;
	}
	else {
		data[index * 2 + 1].inconsistency = m::append(data[index * 2 + 1].inconsistency, data[index].inconsistency);
		data[index * 2 + 2].inconsistency = m::append(data[index * 2 + 2].inconsistency, data[index].inconsistency);
		data[index].inconsistency = m::zero;
	}
}

template<typename Monoid>
void segment_tree<Monoid>::_sift_up(uint index) {
	data[index].value = m::append(data[index * 2 + 1].get(), data[index * 2 + 2].get());
}

template<typename Monoid>
void segment_tree<Monoid>::_sift_all() {
	for (int i = 0; i < _capacity - 1; ++i)
		_sift_down(i);
	for (int i = 0; i < _size; ++i) {
		uint index = i + _capacity - 1;
		if (data[index].filled)
			data[index].value = data[index].inconsistency;
		else
			data[index].value = m::append(data[index].value, data[index].inconsistency);
		data[index].filled = false;
		data[index].inconsistency = m::zero;
	}
	for (int i = _capacity - 2; i >= 0; --i)
		_sift_up(i);
}

template<typename Monoid>
void segment_tree<Monoid>::_resize(uint new_cap) {
	_sift_all();
	uint old_size = _size;
	vector<T> newdata(new_cap);
	for (int i = 0; i < std::min(new_cap, (uint)_size); ++i)
		newdata[i] = data[i + _capacity - 1].value;
	*this = segment_tree(newdata);
	_size = old_size;
}

template<typename Monoid>
segment_tree<Monoid>::segment_tree() : segment_tree(vector<T>()) {
}

template<typename Monoid>
segment_tree<Monoid>::segment_tree(vector<T>& input) {
	_size = input.size();
	_capacity = lpo2(_size);
	data = vector<node>(_capacity * 2 - 1, node(m::zero, 0, 0));
	for (int i = 0; i < _capacity; ++i) {
		if (i < _size)
			data[i + _capacity - 1] = node(input[i], i, i + 1);
		else {
			data[i + _capacity - 1] = node(m::zero, i, i + 1);
		}
	}
	for (int i = _capacity - 2; i >= 0; --i) {
		data[i] = node(m::append(data[i * 2 + 1].value, data[i * 2 + 2].value), data[i * 2 + 1].left, data[i * 2 + 2].right);
	}
}

template<typename Monoid>
uint segment_tree<Monoid>::size() {
	return _size;
}

template<typename Monoid>
bool segment_tree<Monoid>::empty() {
	return _size == 0;
}

template<typename Monoid>
typename Monoid::type segment_tree<Monoid>::_get(uint index, uint l, uint r) {
	if (data[index].left >= r || data[index].right <= l)
		return m::zero;
	if (data[index].left >= l && data[index].right <= r)
		return data[index].get();
	_sift_down(index);
	T ans = m::append(_get(index * 2 + 1, l, r), _get(index * 2 + 2, l, r));
	_sift_up(index);
	return ans;
}

template<typename Monoid>
typename Monoid::type segment_tree<Monoid>::get(uint l, uint r) {
	_check_range(l, r);
	return _get(0, l, r);
}

template<typename Monoid>
typename Monoid::type segment_tree<Monoid>::get(uint index) {
	return get(index, index + 1);
}

template<typename Monoid>
void segment_tree<Monoid>::_add(uint index, uint l, uint r, T summand) {
	if (data[index].left >= r || data[index].right <= l)
		return;
	if (data[index].left >= l && data[index].right <= r) {
		data[index].inconsistency = m::append(data[index].inconsistency, summand);
		return;
	}
	_sift_down(index);
	_add(index * 2 + 1, l, r, summand);
	_add(index * 2 + 2, l, r, summand);
	_sift_up(index);
}

template<typename Monoid>
void segment_tree<Monoid>::add(uint l, uint r, T summand) {
	_check_range(l, r);
	_add(0, l, r, summand);
}

template<typename Monoid>
void segment_tree<Monoid>::add(uint index, T summand) {
	add(index, index + 1, summand);
}

template<typename Monoid>
void segment_tree<Monoid>::_set(uint index, uint l, uint r, T value) {
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

template<typename Monoid>
void segment_tree<Monoid>::set(uint l, uint r, T value) {
	_check_range(l, r);
	_set(0, l, r, value);
}

template<typename Monoid>
void segment_tree<Monoid>::set(uint index, T value) {
	set(index, index + 1, value);
}

template<typename Monoid>
void segment_tree<Monoid>::push_back(T elem) {
	if (_size == _capacity)
		_resize(_capacity * 2);
	_size++;
	set(_size - 1, elem);
}

template<typename Monoid>
void segment_tree<Monoid>::pop_back() {
	if (_size == 0)
		throw std::exception("empty container");
	set(_size - 1, m::zero);
	_size--;
	if (_size * 4 + 1 < _capacity)
		_resize(_capacity / 2);
}