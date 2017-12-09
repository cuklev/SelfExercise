#pragma once

#include <algorithm>
#include <memory>
#include <vector>

// TODO: galloping
template <typename It, typename B, typename Cmp>
void merge(It begin, It middle, It end, B buffer, Cmp cmp) {
	// do less copying
	begin = upper_bound(begin, middle, *middle, cmp);
	end = lower_bound(middle, end, *(middle - 1), cmp);

	const auto LSize = middle - begin;
	const auto RSize = end - middle;

	if(LSize <= RSize) { // left to right merging
		move(begin, middle, buffer);

		auto i = buffer;
		auto buffer_end = i + LSize;
		auto j = middle;
		auto k = begin;

		while(j != end) {
			if(cmp(*i, *j)) *k++ = std::move(*i++);
			else *k++ = std::move(*j++);
		}
		move(i, buffer_end, k);
	} else { // right to left merging
		move(middle, end, buffer);

		auto i = middle - 1;
		auto buffer_start = buffer;
		auto j = buffer_start + RSize - 1;
		auto k = end;

		while(i != begin - 1) {
			if(cmp(*j, *i)) *--k = std::move(*i--);
			else *--k = std::move(*j--);
		}
		move_backward(buffer_start, j + 1, k);
	}
}

inline size_t get_min_run_length(size_t n) {
	while(n >= (1 << 6))
		n = (n + 1) >> 1;
	return n;
}

// is this correct?
template <typename It, typename B, typename Cmp>
void keep_invariant(std::vector<It>& runs, It end, B buffer, Cmp cmp) {
	while(runs.size() >= 3) {
		auto x = runs.back();
		auto y = runs[runs.size() - 2];
		auto z = runs[runs.size() - 3];
		auto x_len = end - x;
		auto y_len = x - y;
		auto z_len = y - z;

		if(z_len <= y_len + x_len) {
			merge(z, y, x, buffer, cmp);
			runs.erase(runs.end() - 2);
		} else if(y_len <= x_len) {
			merge(y, x, end, buffer, cmp);
			runs.pop_back();
		} else {
			break;
		}
	}
}

template <typename It, typename Cmp>
void timsort(It begin, It end, Cmp cmp) {
	using T = std::remove_reference_t<decltype(*begin)>;

	const auto N = end - begin;
	if(N < 2) return; // nothing to sort

	// buffer for merging, allocate once
	auto buffer = std::make_unique<T[]>(N / 2);

	const size_t MinRunLength = get_min_run_length(N);

	std::vector<It> runs;

	// scan for runs
	for(auto it = begin; it != end;) {
		auto run_begin = it;
		runs.push_back(it);
		if(end - it == 1) {
			keep_invariant(runs, end, buffer.get(), cmp);
			break;
		}

		size_t run_length = 2;
		bool decreasing = cmp(*(it + 1), *it);

		it += 2;
		while(it != end && cmp(*it, *(it - 1)) == decreasing) {
			++it;
			++run_length;
		}

		if(decreasing) reverse(run_begin, it);

		while(it != end && run_length < MinRunLength) {
			// binary insertion sort
			auto tmp = std::move(*it);
			auto pos = upper_bound(run_begin, it, tmp, cmp);
			for(auto s_it = it; s_it != pos; --s_it)
				*s_it = std::move(*(s_it - 1));
			*pos = std::move(tmp);
			++it;
			++run_length;
		}

		keep_invariant(runs, it, buffer.get(), cmp);
	}

	// is this correct?
	// fold with merging
	for(auto i = runs.size() - 1; i > 0; --i)
		merge(runs[i - 1], runs[i], end, buffer.get(), cmp);
}

template <typename It>
void timsort(It begin, It end) {
	using T = std::remove_reference_t<decltype(*begin)>;
	timsort(begin, end, std::less<T>());
}
