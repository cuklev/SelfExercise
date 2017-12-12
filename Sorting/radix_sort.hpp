#pragma once

#include <memory>

namespace sortings {

template <typename ItSrc, typename ItDst>
void spread_to_buckets(ItSrc begin, ItSrc const end, int shift_count, size_t* offsets, ItDst output) {
	for(; begin != end; ++begin) {
		auto index = (*begin >> shift_count) & 0xFFFF;
		*(output + offsets[index]) = *begin;
		++offsets[index];
	}
}

template <typename It>
void radix_sort(It begin, It end) {
	using T = std::remove_reference_t<decltype(*begin)>;

	static_assert(std::is_integral_v<T>);
	static_assert(std::is_unsigned_v<T>);

	const auto N = end - begin;
	if(N < 2) return; // nothing to do

	auto buffer = std::make_unique<T[]>(N);
	const auto buf_b = buffer.get();
	const auto buf_e = buffer.get() + N;

	auto counts = std::make_unique<size_t[][1 << 16]>(sizeof(T) / 2);

	for(auto it = begin; it != end; ++it) {
		++counts[0][*it & 0xFFFF];
		if constexpr(sizeof(T) > 2)
			++counts[1][(*it >> 16) & 0xFFFF];
		if constexpr(sizeof(T) > 4)
			++counts[2][(*it >> 32) & 0xFFFF];
		if constexpr(sizeof(T) > 6)
			++counts[3][*it >> 48];
		if constexpr(sizeof(T) > 8)
			throw "not implemented for such types";
	}

	for(int i = 0; i < sizeof(T) / 2; ++i) {
		size_t sum = 0;
		for(auto& x: counts[i]) {
			size_t y = x;
			x = sum;
			sum += y;
		}
	}

	spread_to_buckets(begin, end, 0, counts[0], buf_b);
	if constexpr(sizeof(T) <= 2) {
		std::move(buf_b, buf_e, begin);
		return;
	}

	spread_to_buckets(buf_b, buf_e, 16, counts[1], begin);
	if constexpr(sizeof(T) <= 4) return;

	spread_to_buckets(begin, end, 32, counts[2], buf_b);
	if constexpr(sizeof(T) <= 6) {
		std::move(buf_b, buf_e, begin);
		return;
	}

	spread_to_buckets(buf_b, buf_e, 48, counts[3], begin);
}

}
