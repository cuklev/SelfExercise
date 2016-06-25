#include<iostream>

#include "avl.h"

int main()
{
	Avl<int> tree;
	tree.insert(5);
	tree.insert(1);
	tree.insert(2);
	tree.insert(4);
	tree.insert(7);
	tree.insert(6);
	tree.insert(0);

	tree.foreach([](int x) {
		std::cout << x << ' ';
	});
}
