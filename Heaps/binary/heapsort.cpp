#include "binary-heap.h"

#include<iostream>
#include<random>
#include<vector>
#include<algorithm>

int main() {
	const int N = 10000;
	std::vector<int> numbers(N);

	for(int i = 0; i < N; ++i)
		numbers[i] = i;

	std::mt19937_64 rng {std::random_device{}()};
	std::shuffle(numbers.begin(), numbers.end(), rng);

	BinaryHeap<int> heap;
	for(auto& x : numbers) heap.push(x);

	bool works = true;

	for(int i = 0; i < N; ++i) {
		numbers[i] = heap.top();
		heap.pop();
		if(numbers[i] != i) {
			works = false;
			break;
		}
	}

	std::cout << (works ? "Works" : "Doesn't work") << '\n';
}
