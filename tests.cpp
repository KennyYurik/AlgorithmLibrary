#include "segment_tree.h"
#include <chrono>

void main() {
	std::ofstream out("out.txt");
	vector<int> data;
	int n = 10000;
	for (int i = 0; i < n; ++i) {
		data.push_back(i);
	}
	segment_tree tree(data);
	for (int i = 0; i < n; ++i) {
		int r = rand() % n;
		int l = rand() % (r + 1);
		int ans = tree.get(l, r);
		if (std::max(ans, 0) != r * (r - 1) / 2 - l * (l - 1) / 2) {
			out << i << " " << ans << " " << l << " " << r;
			return;
		}
	}
}