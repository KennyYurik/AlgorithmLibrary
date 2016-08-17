#include "segment_tree.h"
#include <chrono>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <fstream>
typedef std::chrono::high_resolution_clock mclock;

double convert_duration(mclock::duration d) {
	return std::chrono::duration_cast<std::chrono::duration<double>>(d).count();
}

// TODO : SPLIT TO CORRECTNESS TEST AND PERFORMANCE TEST
void test_segment_tree(std::ofstream& log, uint data_size, uint query_size, int max_element) {
	log << "--- segment tree ---" << std::endl;
	log << "data size: " << data_size << std::endl;
	log << "queries: " << data_size << std::endl;
	srand(clock());
	auto st_duration = mclock::duration::zero();
	auto v_duration = mclock::duration::zero();
	mclock::time_point st_time, v_time;

	// vector init
	v_time = mclock::now();
	vector<int> data;
	for (int i = 0; i < data_size; ++i)
		data.push_back(rand() % max_element);
	log << "vector initialization time: " << convert_duration(mclock::now() - v_time) << std::endl;

	// segment_tree init
	st_time = mclock::now();
	segment_tree tree(data);
	log << "segment initialization time: " << convert_duration(mclock::now() - st_time) << std::endl;

	// queries
	uint bad_counter = 0;
	for (int i = 0; i < query_size; ++i) {
		uint r = rand() % data_size + 1;
		uint l = rand() % r;
		uint query = rand() % 3;
		if (query == 0) {
			// segment tree operation
			st_time = mclock::now();
			int ans_tree = tree.get(l, r);
			st_duration += mclock::now() - st_time;

			// vector operation
			v_time = mclock::now();
			int true_ans = std::accumulate(data.begin() + l, data.begin() + r, 0);
			v_duration += mclock::now() - v_time;
			
			if (ans_tree != true_ans)
				bad_counter++;
		}
		else if (query == 1) {
			uint value = rand() % max_element;
			
			// segment tree operation
			st_time = mclock::now();
			tree.set(l, r, value);
			st_duration += mclock::now() - st_time;

			// vector operation
			v_time = mclock::now();
			std::fill(data.begin() + l, data.begin() + r, value);
			v_duration += mclock::now() - v_time;
		}
		else if (query == 2) {
			uint value = rand() % max_element;
			
			// segment tree operation
			st_time = mclock::now();
			tree.add(l, r, value);
			st_duration += mclock::now() - st_time;

			// vector operation
			v_time = mclock::now();
			for (auto it = data.begin() + l; it != data.begin() + r; ++it)
				*it += value;
			v_duration += mclock::now() - v_time;
		}
	}
	log << "correctness: " << 100.0 * (query_size - bad_counter) / (query_size) << "%" << std::endl;
	log << "vector time: " << convert_duration(v_duration) << std::endl;
	log << "segment tree time: " << convert_duration(st_duration)  << std::endl;
	log << "--- segment tree ---" << std::endl << std::endl;
}

void main() {
	std::ofstream log("test_log.txt");
	vector<int> data;
	test_segment_tree(log, 100000, 1000, 1000000);
}