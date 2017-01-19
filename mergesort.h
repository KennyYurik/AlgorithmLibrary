#pragma once
#include <algorithm>
using std::swap_ranges;
using std::iter_swap;


/*
	selection sort
	each block should be sorted
	k = block count, d = block size
	O(k * d) swaps
	O(k * k + k * d) time
*/
template<class Iter>
inline void slow_sort_blocks(Iter begin, Iter end, int bsize) {
	for (auto it = begin; it != end; it += bsize) {
		Iter min = it;
		for (auto it2 = it; it2 != end; it2 += bsize) {
			if (*it2 < *min || *it2 == *min && *(it2 + (bsize - 1)) < *(min + (bsize - 1)))
				min = it2;
		}
		swap_ranges(it, it + bsize, min);
	}
}

template<class Iter>
inline void slow_sort(Iter begin, Iter end) {
	slow_sort_blocks(begin, end, 1);
}

/*
	merge begin...mid and mid...end using buffer
	buffer size should be >= begin...mid
*/
template<class Iter>
inline void merge(Iter begin, Iter mid, Iter end, Iter buffer) {
	auto buff_end = swap_ranges(begin, mid, buffer);
	auto begin1 = mid, end1 = end, begin2 = buffer, end2 = buff_end, out = begin;
	while (true){
		if (begin1 == end1) {
			swap_ranges(begin2, end2, out);
			return;
		}
		if (begin2 == end2) {
			swap_ranges(begin1, end1, out);
			return;
		}
		if (*begin2 < *begin1) {
			iter_swap(out, begin2);
			++begin2;
		}
		else {
			iter_swap(out, begin1);
			++begin1;
		}
		++out;
	}
}

template<class Iter>
void merge_sort(Iter begin, Iter end) {
	int length = end - begin;

	// use quadratic sort on small array
	if (length < 16) {
		slow_sort(begin, end);
		return;
	}
	Iter mid = begin + length / 2;
	
	merge_sort(begin, mid);
	merge_sort(mid, end);
	
	// inplace merge, O(n)
	int bsize = std::sqrt(length);
	int bcount = length / bsize;
	Iter cut_block = begin + ((mid - begin) / bsize) * bsize;
	Iter buffer = begin + bsize * (bcount - 1);
	swap_ranges(cut_block, cut_block + bsize, buffer);
	bcount--;
	
	// bsize blocks, each is sorted + buffer, sizeof(buffer) >= bsize
	slow_sort_blocks(begin, buffer, bsize);
	for (auto it = begin; it + bsize != buffer; it += bsize) {
		merge(it, it + bsize, it + 2 * bsize, buffer);
	}

	slow_sort(buffer, end);
	
	// begin...buffer now sorted 
	// buffer...end now sorted
	// lets merge new blocks with size = buffer...end
	// new buffer is at the beginning
	bsize = end - buffer;
	bcount = length / bsize - 1;
	buffer = begin;
	Iter buff_end = end - bsize * bcount;

	for (auto it = end - 2 * bsize; it >= buff_end; it -= bsize) {
		merge(it, it + bsize, it + 2 * bsize, buffer);
	}
	slow_sort(buffer, buff_end + bsize);
}