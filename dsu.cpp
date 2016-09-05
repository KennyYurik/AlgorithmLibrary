#include "dsu.h"

dsu::dsu(int n) {
	p = vector<int>(n);
	for (int i = 0; i < n; ++i) {
		p[i] = i;
	}
}

int dsu::add() {
	p.push_back(p.size() - 1);
	return p.back();
}

int dsu::find(int a) {
	if (a < 0 || a >= p.size())
		return -1;
	if (a != p[a])
		p[a] = find(p[a]);
	return p[a];
}

void dsu::unite(int a, int b) {
	if (find(a) != find(b)) {
		if (rand() % 2)
			p[a] = b;
		else
			p[b] = a;
	}
}