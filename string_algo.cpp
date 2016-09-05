#include "string_algo.h"
#include <algorithm>

vector<int> prefix_function(string &s) {
	vector<int> p(s.length(), 0);
	for (int i = 1; i < s.length(); ++i) {
		int k = p[i - 1];
		while (k > 0 && s[i] != s[k]) {
			k = p[k - 1];
		}
		if (s[i] == s[k])
			k++;
		p[i] = k;
	}
	return p;
}

vector<int> z_function(string &s) {
	vector<int> z(s.length(), 0);
	int left = 0, right = 0;
	for (int i = 1; i < s.length(); ++i) {
		z[i] = std::max(0, std::min(right - i, z[i - left]));
		while (i + z[i] < s.length() && s[z[i]] == s[i + z[i]])
			z[i]++;
		if (i + z[i] >= right) {
			left = i;
			right = i + z[i];
		}
	}
	return z;
}