#include<iostream>

#include "avl.h"

int main()
{
	Avl<int> tree;
	for(int i = 0; i < 1000000; ++i)
		tree.insert(i);

	std::cout << tree.size() << ' ' << tree.height() << '\n';
}
