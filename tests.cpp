#pragma comment(linker, "/STACK:25600000")
#include "segment_tree.h"
#include <chrono>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
typedef std::chrono::high_resolution_clock mclock;

double convert_duration(mclock::duration d) {
	return std::chrono::duration_cast<std::chrono::duration<double>>(d).count();
}

void test_correctness_segment_tree(std::ofstream& log, uint start_size, uint queries) {
	log << "--- segment tree correctness ---" << std::endl;
	log << "start size: " << start_size << std::endl;
	log << "queries: " << queries << std::endl;
	srand(clock());
	
	// vector init
	vector<int> data;
	for (int i = 0; i < start_size; ++i)
		data.push_back(rand());
	
	// segment_tree init
	segment_tree tree(data);
	
	// queries
	uint bad_counter = 0;
	uint query_counter[] = { 0, 0, 0, 0, 0 };
	std::string query_names[] = { "get", "set", "add", "push", "pop" };
	for (int i = 0; i < queries; ++i) {
		uint query = rand() % 5;
		if (data.size() == 0)
			query = 3;
		if (query == 0) {
			uint r = rand() % data.size() + 1;
			uint l = rand() % r;
			
			// segment tree operation
			int ans_tree = tree.get(l, r);
	
			// vector operation
			int true_ans = std::accumulate(data.begin() + l, data.begin() + r, 0);
			
			if (ans_tree != true_ans)
				bad_counter++;
		}
		else if (query == 1) {
			uint r = rand() % data.size() + 1;
			uint l = rand() % r;
			uint value = rand();
			
			// segment tree operation
			tree.set(l, r, value);
	
			// vector operation
			std::fill(data.begin() + l, data.begin() + r, value);
		}
		else if (query == 2) {
			uint r = rand() % data.size() + 1;
			uint l = rand() % r;
			uint value = rand();
			
			// segment tree operation
			tree.add(l, r, value);
		
			// vector operation
			for (auto it = data.begin() + l; it != data.begin() + r; ++it)
				*it += value;
		}
		else if (query == 3) {
			int value = rand();
			// segment tree operation
			tree.push_back(value);
		
			// vector operation
			data.push_back(value);
		}
		else if (query == 4) {
			if (data.empty())
				continue;
			// segment tree operation
			tree.pop_back();
		
			// vector operation
			data.pop_back();
		}
		query_counter[query]++;
	}
	log << "correctness: " << 100.0 * (queries - bad_counter) / (queries) << "%" << std::endl;
	for (int i = 0; i < 5; ++i) {
		log << query_names[i] << ": " << query_counter[i] << std::endl;
	}
	log << "end size: " << data.size() << " " << tree.size() << std::endl;
	log << "--- segment tree ---" << std::endl << std::endl;
}

void main() {
	std::ofstream log("test_log.txt");
	test_correctness_segment_tree(log, 1000000, 100);
}