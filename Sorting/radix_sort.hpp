#pragma once

#include <algorithm>
#include <type_traits>
#include <queue>
#include <memory>

namespace sortings {

template <typename T, size_t BC>
class Buckets {
	private:
		std::unique_ptr<std::queue<T>[]> buckets_ = std::make_unique<std::queue<T>[]>(BC);

		// iterator-like thingy
		class iterator {
			private:
				std::queue<T>* curr_bucket_;
				std::queue<T>* const end_bucket_;

			public:
				iterator(std::queue<T>* c, std::queue<T>* e)
					: curr_bucket_(c)
					, end_bucket_(e) {}

				iterator& operator++() {
					while(curr_bucket_ < end_bucket_ && curr_bucket_->empty())
						++curr_bucket_;
					return *this;
				}

				T operator*() {
					T x = curr_bucket_->front();
					curr_bucket_->pop();
					return x;
				}

				bool operator!=(iterator const& other) {
					return curr_bucket_ != other.curr_bucket_;
				}
		};

	public:
		iterator begin() {
			auto ptr = buckets_.get();
			iterator it {ptr, ptr + BC};
			return ++it;
		}

		iterator end() {
			auto ptr = buckets_.get() + BC;
			return {ptr, ptr};
		}

		void push(size_t index, T number) {
			buckets_[index].push(number);
		}
};

template <typename It, typename F>
auto spread_to_buckets(It begin, It const end, F get_bucket) {
	using T = std::remove_reference_t<decltype(*begin)>;
	Buckets<T, 1 << 16> new_buckets;

	for(; begin != end; ++begin) {
		auto number = *begin;
		auto index = get_bucket(number);
		new_buckets.push(index, number);
	}

	return new_buckets;
}

template <typename It>
void radix_sort(It begin, It end) {
	using T = std::remove_reference_t<decltype(*begin)>;

	static_assert(std::is_integral_v<T>);
	static_assert(std::is_unsigned_v<T>);

	if(end - begin < 2) return; // nothing to do

	auto buckets = spread_to_buckets(begin, end,
			[](T x) { return x & 0xFFFF; });

	for(unsigned i = 12; i < sizeof(T) * 8; i += 12) {
		buckets = spread_to_buckets(buckets.begin(), buckets.end(),
				[i](T x) { return (x >> i) & 0xFFF; });
	}

	for(auto it = buckets.begin(); begin != end; ++it)
		*begin++ = *it;
}

}
