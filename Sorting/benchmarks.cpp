#include "timsort.hpp"
#include "radix_sort.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
#include <cassert>
#include <iterator>
#include <cstdlib>

template <typename T, typename It = typename std::vector<T>::iterator>
void time_test(std::vector<T> stuff, const char* name, void (*sort_function)(It, It)) {
	auto orig_stuff = stuff;
	std::sort(orig_stuff.begin(), orig_stuff.end());

	std::cout << std::left << std::setw(20) << name;

	// on unsorted
	auto begin = std::chrono::system_clock::now();
	sort_function(stuff.begin(), stuff.end());
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> duration = end - begin;
	std::cout << duration.count() << "s\n";

	assert(stuff == orig_stuff);

	// on sorted
	begin = std::chrono::system_clock::now();
	sort_function(stuff.begin(), stuff.end());
	end = std::chrono::system_clock::now();

	duration = end - begin;
	std::cout
		<< std::left << std::setw(20) << "   on sorted ->"
		<< duration.count() << "s\n";

	assert(stuff == orig_stuff);
}

template <typename It>
void _qsort(It begin, It end) {
	using T = std::remove_reference_t<decltype(*begin)>;
	qsort(&*begin, end - begin, sizeof(T),
			[] (const void* x, const void* y) -> int {
				return *reinterpret_cast<const T*>(x) > *reinterpret_cast<const T*>(y);
			});
}

int main() {
	std::random_device rd;
	std::mt19937_64 rng(rd());
	using T = decltype(rng());

	for(const size_t N: {10, /*100, 1000,*/ 10000, 100000, 1000000}) {
		std::cout << "N = " << N << '\n';

		std::vector<T> numbers;
		generate_n(back_inserter(numbers), N, rng);

		time_test(numbers, "qsort", _qsort);
		time_test(numbers, "std::sort", std::sort);
		time_test(numbers, "std::stable_sort", std::stable_sort);
		time_test(numbers, "timsort", sortings::timsort);
		time_test(numbers, "radix_sort", sortings::radix_sort);
	}
}
