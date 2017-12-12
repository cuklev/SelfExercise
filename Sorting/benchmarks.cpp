#include "timsort.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
#include <cassert>
#include <iterator>
#include <cstdlib>

template <typename T>
void time_test(std::vector<T> stuff, void (*sort_function)(typename std::vector<T>::iterator, typename std::vector<T>::iterator)) {
	auto orig_stuff = stuff;
	std::sort(orig_stuff.begin(), orig_stuff.end());

	auto begin = std::chrono::system_clock::now();
	sort_function(stuff.begin(), stuff.end());
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> duration = end - begin;
	std::cout << duration.count() << '\n';

	assert(stuff == orig_stuff);
}

#define time_test(x, s) \
	std::cout \
		<< std::left \
		<< std::setw(20) \
		<< #s << ' '; \
	time_test(x, s)

int main() {
	std::random_device rd;
	std::mt19937_64 rng(rd());
	using T = decltype(rng());

	for(auto N: {10, 100, 1000, 10000, 100000, 1000000}) {
		std::cout << "N = " << N << '\n';

		std::vector<T> numbers;
		generate_n(back_inserter(numbers), N, rng);

		time_test(numbers, std::sort);
		time_test(numbers, std::stable_sort);
		time_test(numbers, timsort);

		{ // qsort is ugly
			auto begin = std::chrono::system_clock::now();
			std::qsort(&numbers[0], numbers.size(),
					sizeof(numbers[0]),
					[] (const void* x, const void* y) -> int {
						return *reinterpret_cast<const T*>(x) < *reinterpret_cast<const T*>(y);
					});
			auto end = std::chrono::system_clock::now();

			std::chrono::duration<double> duration = end - begin;
			std::cout
				<< std::left
				<< std::setw(20)
				<< "qsort" << ' '
				<< duration.count() << '\n';
		}
	}
}
