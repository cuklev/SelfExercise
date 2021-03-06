#include "btree.h"
#include<algorithm>
#include<iostream>
#include<random>

int main() {
	std::mt19937_64 rng {std::random_device{}()};

	BTree<int, 4> tree;

	std::vector<int> v;
	for(int i = 0; i < 10; ++i)
		v.push_back(i);

	std::shuffle(v.begin(), v.end(), rng);

	for(int x : v) {
		std::cout << ' ' << x;
		tree.insert(x);
	}
	std::cout << '\n';

	for(int x : tree)
		std::cout << ' ' << x;
	std::cout << '\n';

	for(auto it = tree.end(); it != tree.begin();) {
		--it;
		std::cout << ' ' << *it;
	}
	std::cout << '\n';

	std::cout << (tree.lower_bound(0) == tree.begin()) << '\n';
	std::cout << (tree.lower_bound(9) == tree.end()) << '\n';
	std::cout << (tree.lower_bound(10) == tree.end()) << '\n';

	std::cout << (tree.find(0) == tree.begin()) << '\n';
	std::cout << (tree.find(9) == tree.end()) << '\n';
	std::cout << (tree.find(10) == tree.end()) << '\n';
}
