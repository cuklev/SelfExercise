#include<iostream>
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
 * ROOT is primitive 2^N-th root modulo MOD
 */

const uint64_t ROOT = PowMod(search(), K);

int main()
{
	assert(isprime(K));
	assert(isprime(MOD));

	std::cout << ROOT << ' ' << MOD << '\n';
}
