#pragma comment(linker, "/STACK:25600000")
#include "segment_tree.h"
#include <chrono>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
typedef std::chrono::steady_clock sclock;

double convert_duration(sclock::time_point start, sclock::time_point end) {
	return std::chrono::duration<double, std::milli>(end - start).count();
}

void test_performance_segment_tree(std::ofstream& log, uint data_size, uint queries) {
	typedef unsigned long long ull;
	log << "--- segment tree performance ---" << std::endl;
	log << "data size: " << data_size << std::endl;
	log << "queries: " << queries << std::endl;
	srand(clock());
	
	// vector init
	sclock::time_point vec_start = sclock::now();
	vector<ull> data;
	for (int i = 0; i < data_size; ++i)
		data.push_back(i);
	sclock::time_point vec_end = sclock::now();
	log << "vector creation time: " << convert_duration(vec_start, vec_end) << " ms" << std::endl;
	
	// segment_tree init
	sclock::time_point tree_start = sclock::now();
	segment_tree<monoid_add<ull>> tree(data);
	sclock::time_point tree_end = sclock::now();
	log << "tree creation time: " << convert_duration(tree_start, tree_end) << " ms" << std::endl;


	// queries
	sclock::time_point query_start = sclock::now();
	uint bad_counter = 0;
	for (int i = 0; i < queries; ++i) {
		uint r = rand() % data.size() + 1;
		uint l = rand() % r;
		if (tree.get(l, r) != r * (r - 1) / 2 - l * (l - 1) / 2) {
			bad_counter++;
		}
	}
	sclock::time_point query_end = sclock::now();
	log << "queries time: " << convert_duration(query_start, query_end) << " ms" << std::endl;
	log << "correctness: " << 100.0 * (queries - bad_counter) / (queries) << "%" << std::endl;
	log << "--- segment tree ---" << std::endl << std::endl;
}

void main() {
	std::ofstream log("test_log.txt");
	test_performance_segment_tree(log, 10000000, 1000000);
}