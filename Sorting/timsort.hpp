#pragma once

#include <algorithm>
#include <memory>
#include <vector>

namespace sortings {

inline size_t get_min_run_length(size_t n) {
	while(n >= (1 << 6))
		n = (n + 1) >> 1;
	return n;
}

template <typename It, typename T, typename Cmp>
class TimSortClass {
	static constexpr int MinGallop = 7;

	int min_gallop_ = MinGallop;
	const Cmp cmp_;
	std::vector<It> runs_;
	std::unique_ptr<T[]> merge_buffer_;

	TimSortClass(Cmp cmp) : cmp_(cmp) {}

	template <typename It_>
	It_ gallop_left_left(It_ begin, It_ end, T const& value) {
		auto it = begin + 1;
		size_t offset = 2;

		while(it < end) {
			if(cmp_(value, *it)) break;
			begin = it;
			it += offset;
			offset *= 2;
		}

		if(it > end) it = end;

		return std::upper_bound(begin, it, value, cmp_);
	}

	template <typename It_>
	It_ gallop_left_right(It_ begin, It_ end, T const& value) {
		auto it = begin + 1;
		size_t offset = 2;

		while(it < end) {
			if(!cmp_(*it, value)) break;
			begin = it;
			it += offset;
			offset *= 2;
		}

		if(it > end) it = end;

		return std::lower_bound(begin, it, value, cmp_);
	}

	template <typename It_>
	It_ gallop_right_left(It_ begin, It_ end, T const& value) {
		auto it = end - 2;
		size_t offset = 2;

		while(it >= begin) {
			if(!cmp_(value, *it)) break;
			end = it + 1;
			it -= offset;
			offset *= 2;
		}

		if(it < begin) it = begin - 1;

		return std::upper_bound(it + 1, end, value, cmp_);
	}

	template <typename It_>
	It_ gallop_right_right(It_ begin, It_ end, T const& value) {
		auto it = end - 2;
		size_t offset = 2;

		while(it >= begin) {
			if(cmp_(*it, value)) break;
			end = it + 1;
			it -= offset;
			offset *= 2;
		}

		if(it < begin) it = begin - 1;

		return std::lower_bound(it + 1, end, value, cmp_);
	}

	// TODO: fix galloping
	void merge_with_gallop(It begin, It middle, It end) {
		begin = gallop_left_left(begin, middle, *middle);
		end = gallop_right_right(middle, end, *(middle - 1));

		const auto LSize = middle - begin;
		const auto RSize = end - middle;

		if(LSize <= RSize) { // left to right merging
			std::move(begin, middle, merge_buffer_.get());

			auto i = merge_buffer_.get();
			auto buffer_end = i + LSize;
			auto j = middle;
			auto k = begin;

			int ci = 0, cj = 0;
			while(j != end) {
				if(cmp_(*i, *j)) {
					*k++ = std::move(*i++);
					++ci; cj = 0;
					if(ci >= min_gallop_)
						goto galloping_left_left;
				} else {
					*k++ = std::move(*j++);
					ci = 0; ++cj;
					if(cj >= min_gallop_)
						goto galloping_left_right;
				}
				continue;

				do {
					min_gallop_ -= min_gallop_ > 1;

galloping_left_left:
					{
						auto it = gallop_left_left(i, buffer_end, *j);
						k = std::move(i, it, k);
						ci = it - i;
						i = it;
					}

					if(ci < MinGallop) break;
					min_gallop_ -= min_gallop_ > 1;

galloping_left_right:
					{
						auto it = gallop_left_right(j, end, *i);
						k = std::move(j, it, k);
						cj = it - j;
						j = it;
					}
				} while(cj >= MinGallop);

				++min_gallop_;
			}

			std::move(i, buffer_end, k);
		} else { // right to left merging
			std::move(middle, end, merge_buffer_.get());

			auto i = middle - 1;
			auto buffer_start = merge_buffer_.get();
			auto j = buffer_start + RSize - 1;
			auto k = end;

			int ci = 0, cj = 0;
			while(i + 1 != begin) {
				if(cmp_(*j, *i)) {
					*--k = std::move(*i--);
					++ci; cj = 0;
					if(ci >= min_gallop_)
						goto galloping_right_left;
				} else {
					*--k = std::move(*j--);
					ci = 0; ++cj;
					if(cj >= min_gallop_)
						goto galloping_right_right;
				}
				continue;

				do {
					min_gallop_ -= min_gallop_ > 1;

galloping_right_left:
					{
						auto it = gallop_right_left(begin, i + 1, *j);
						k = std::move_backward(it, i + 1, k);
						ci = i - it + 1;
						i = it - 1;
					}

					if(ci < MinGallop) break;
					min_gallop_ -= min_gallop_ > 1;
galloping_right_right:
					{
						auto it = gallop_right_right(buffer_start, j + 1, *i);
						k = std::move_backward(it, j + 1, k);
						cj = j - it + 1;
						j = it - 1;
					}
				} while(cj >= MinGallop);

				++min_gallop_;
			}

			std::move_backward(buffer_start, j + 1, k);
		}
	}

	void merge(It begin, It middle, It end) {
		begin = gallop_left_left(begin, middle, *middle);
		end = gallop_right_right(middle, end, *(middle - 1));

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

			if(y - z > end - y)
				break;

			merge(z, y, x);
			runs_.erase(runs_.end() - 2);
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

}
