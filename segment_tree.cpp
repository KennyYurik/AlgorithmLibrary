#include <vector>
#include <algorithm>
#include <fstream>
using std::vector;
typedef unsigned int uint;

class segment_tree {
private:
	struct node {
		uint left, right;
		int value;
		
		node(){};
		node(int value, uint l, uint r) : value(value), left(l), right(r) {};
	}; 
	
	vector<node> data;
	uint size;
	uint capacity;
	
	// least power of 2 greater or equal than n
	uint lpo2(uint n) { 
		n--;
		for (int i = 1; i < 32; i *= 2) {
			n |= n >> i;
		}
		return ++n;
	}
	
	int _get(uint index, uint l, uint r) {
		if (data[index].left >= r || data[index].right <= l)
			return 0;
		if (data[index].left >= l && data[index].right <= r)
			return data[index].value;
		return _get(index * 2 + 1, l, r) + _get(index * 2 + 2, l, r);
	}

public:
	segment_tree() {
		size = 0;
		capacity = 0;
		data = vector<node>();
	}

	segment_tree(vector<int>& input) {
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

	int get(uint l, uint r) {
		if (l < 0)
			l = 0;
		if (r > size)
			r = size;
		if (l >= r)
			l = r;
		return _get(0, l, r);
	}
};

void main() {
	std::ofstream out("out.txt");
	vector<int> data;
	int n = 1000000;
	for (int i = 0; i < n; ++i) {
		data.push_back(i);
	}
	segment_tree tree(data);
	int ans = tree.get(100, 100);
	int ans2 = ans;
	/*for (int i = 0; i < n; ++i) {
		int l = rand() % n, r = rand() % n;
		int ans = tree.get(l, r);
		if (std::max(ans, 0) != r * (r - 1) / 2 - l * (l - 1) / 2) {
			out << i << " " <<  ans << " " << l << " " << r;
			return;
		}
	}*/
}