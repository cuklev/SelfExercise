// This program multiplies two numbers using fast fourier transform

#include<iostream>
#include<vector>
#include<cassert>

/*
 * MOD should be bigger than (N + M) * (B - 1) ^ 2
 *   where N and M are the lengths of the numbers
 *         B is the base (usually 10)
 *     should be K * 2 ^ N + 1
 *    	  in order for N-th primitive root to exist
 *     should be prime
 */

const uint64_t K = 13;
const uint64_t N = 28;
const uint64_t MOD = K * (1ll << N) + 1;

constexpr uint64_t PowMod(uint64_t a, uint64_t p)
{
	if(p == 0) return 1;
	
	auto b = PowMod(a, p / 2);
	b = b * b % MOD;

	return (p & 1) ? (a * b % MOD) : b;
}

inline uint64_t DivMod(uint64_t a)
{
	return PowMod(a, MOD - 2);
}

constexpr uint64_t search()
{
	for(uint64_t root = 1; ; ++root)
	{
		if(PowMod(root, MOD / 2) == 1)
			continue;
		if(PowMod(root, MOD / K) == 1)
			continue;
		if(PowMod(root, MOD - 1) != 1)
			continue;
		return root;
	}

	return 0;
}

constexpr bool isprime(uint64_t number)
{
	if(number % 2 == 0) return false;

	for(uint64_t p = 3; p * p <= number; p += 2)
		if(number % p == 0)
			return false;

	return true;
}

/*
 * Roots[index] is primitive 2^index-th root modulo MOD
 */

uint64_t Roots[32];

inline void precompute()
{
	assert(isprime(K));
	assert(isprime(MOD));

	Roots[N] = PowMod(search(), K);
	for(int i = N; i > 0; --i)
		Roots[i - 1] = PowMod(Roots[i], 2);
}

const unsigned Mask1 = 0x55555555;
const unsigned Mask2 = 0x33333333;
const unsigned Mask3 = 0x0F0F0F0F;
const unsigned Mask4 = 0x00FF00FF;
const unsigned Mask5 = 0x0000FFFF;

inline unsigned reverse_bits(unsigned x, int bits)
{
	x = ((Mask1 & x) <<  1) | ((x >>  1) & Mask1);
	x = ((Mask2 & x) <<  2) | ((x >>  2) & Mask2);
	x = ((Mask3 & x) <<  4) | ((x >>  4) & Mask3);
	x = ((Mask4 & x) <<  8) | ((x >>  8) & Mask4);
	x = ((Mask5 & x) << 16) | ((x >> 16) & Mask5);

	return x >> (32 - bits);
}

using Samples = std::vector<uint64_t>;

void fft(Samples &X, int bits, bool reverse = false)
{
	for(int i = 0; i < (1 << bits); ++i)
	{
		int j = reverse_bits(i, bits);
		if(i < j) std::swap(X[i], X[j]);
	}

	for(int b = 0; b < bits; ++b)
	{
		const uint64_t root = reverse ? DivMod(Roots[b + 1]) : Roots[b + 1];

		for(unsigned i = 0; i < X.size(); i += (2 << b))
		{
			uint64_t w = 1;
			for(unsigned j = i; j < i + (1 << b); ++j)
			{
				X[j + (1 << b)] = X[j + (1 << b)] * w % MOD;
				auto temp = MOD + X[j] - X[j + (1 << b)];
				X[j] = (X[j] + X[j + (1 << b)]) % MOD;
				X[j + (1 << b)] = temp % MOD;

				w = root * w % MOD;
			}
		}
	}
}

int main()
{
	std::cin.tie(0);
	std::ios::sync_with_stdio(0);

	precompute();

	std::string num1, num2;
	std::cin >> num1 >> num2;

	int bits = 32 - __builtin_clz(num1.size() + num2.size() - 1);
	int n = 1 << bits;

	Samples samples1(n), samples2(n);

	for(unsigned i = 0; i < num1.size(); ++i)
		samples1[i] = num1[num1.size() - 1 - i] - '0';
	fft(samples1, bits);

	for(unsigned i = 0; i < num2.size(); ++i)
		samples2[i] = num2[num2.size() - 1 - i] - '0';
	fft(samples2, bits);

	Samples product(n);
	for(int i = 0; i < n; ++i)
		product[i] = samples1[i] * samples2[i] % MOD;

	fft(product, bits, true);

	std::vector<int> answer(n);
	for(int i = 0; i < n; ++i)
		answer[i] = product[i] * DivMod(n) % MOD;

	for(int i = 1; i < n; ++i)
	{
		answer[i] += answer[i - 1] / 10;
		answer[i - 1] %= 10;
	}

	while(answer.size() > 1 && answer.back() == 0)
		answer.pop_back();

	for(unsigned i = 0; i < answer.size(); ++i)
		std::cout << answer[answer.size() - 1 - i];
	std::cout << '\n';
}
