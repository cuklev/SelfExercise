#pragma once

#include <algorithm>
#include <memory>
#include <vector>

inline size_t get_min_run_length(size_t n) {
	while(n >= (1 << 6))
		n = (n + 1) >> 1;
	return n;
}

template <typename It, typename T, typename Cmp>
class TimSortClass {
	const Cmp cmp_;
	std::vector<It> runs_;
	std::unique_ptr<T[]> merge_buffer_;

	TimSortClass(Cmp cmp) : cmp_(cmp) {}

	// TODO: galloping
	void merge(It begin, It middle, It end) {
		begin = std::upper_bound(begin, middle, *middle, cmp_);
		end = std::lower_bound(middle, end, *(middle - 1), cmp_);

		const auto LSize = middle - begin;
		const auto RSize = end - middle;

		if(LSize <= RSize) { // left to right merging
			std::move(begin, middle, merge_buffer_.get());

			auto i = merge_buffer_.get();
			auto buffer_end = i + LSize;
			auto j = middle;
			auto k = begin;

			while(j != end) {
				if(cmp_(*i, *j)) *k++ = std::move(*i++);
				else *k++ = std::move(*j++);
			}
			std::move(i, buffer_end, k);
		} else { // right to left merging
			std::move(middle, end, merge_buffer_.get());

			auto i = middle - 1;
			auto buffer_start = merge_buffer_.get();
			auto j = buffer_start + RSize - 1;
			auto k = end;

			while(i + 1 != begin) {
				if(cmp_(*j, *i)) *--k = std::move(*i--);
				else *--k = std::move(*j--);
			}
			std::move_backward(buffer_start, j + 1, k);
		}
	}

	void keep_invariant(It end) {
		while(runs_.size() >= 3) {
			auto x = runs_[runs_.size() - 1];
			auto y = runs_[runs_.size() - 2];
			auto z = runs_[runs_.size() - 3];

			if(y - z <= end - y) {
				merge(z, y, x);
				runs_.erase(runs_.end() - 2);
			} else {
				break;
			}
		}
	}

	inline void operator()(It begin, It end) {
		const auto N = end - begin;
		if(N < 2) return; // nothing to sort

		// buffer for merging, allocate once
		merge_buffer_ = std::make_unique<T[]>(N >> 1);

		const size_t MinRunLength = get_min_run_length(N);

		// scan for runs
		for(auto it = begin; it != end;) {
			auto run_begin = it;
			runs_.push_back(it);
			if(end - it == 1) {
				keep_invariant(end);
				break;
			}

			size_t run_length = 2;
			bool decreasing = cmp_(*(it + 1), *it);

			it += 2;
			while(it != end && cmp_(*it, *(it - 1)) == decreasing) {
				++it;
				++run_length;
			}

			if(decreasing) std::reverse(run_begin, it);

			while(it != end && run_length < MinRunLength) {
				// binary insertion sort
				auto tmp = std::move(*it);
				auto pos = upper_bound(run_begin, it, tmp, cmp_);
				for(auto s_it = it; s_it != pos; --s_it)
					*s_it = std::move(*(s_it - 1));
				*pos = std::move(tmp);
				++it;
				++run_length;
			}

			keep_invariant(it);
		}

		// is this correct?
		// fold with merging
		for(auto i = runs_.size() - 1; i > 0; --i)
			merge(runs_[i - 1], runs_[i], end);
	}

	template<typename _It, typename _Cmp>
	friend void timsort(_It, _It, _Cmp);
};


template <typename It, typename Cmp>
void timsort(It begin, It end, Cmp cmp) {
	using T = std::remove_reference_t<decltype(*begin)>;
	TimSortClass<It, T, Cmp>{cmp}(begin, end);
}

template <typename It>
void timsort(It begin, It end) {
	using T = std::remove_reference_t<decltype(*begin)>;
	timsort(begin, end, std::less<T>());
}
