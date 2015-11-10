// This program multiplies two numbers using fast fourier transform

#include<iostream>
#include<cmath>
#include<complex>
#include<vector>

const unsigned Mask1 = 0b01010101010101010101010101010101;
const unsigned Mask2 = 0b00110011001100110011001100110011;
const unsigned Mask3 = 0b00001111000011110000111100001111;
const unsigned Mask4 = 0b00000000111111110000000011111111;
const unsigned Mask5 = 0b00000000000000001111111111111111;

inline unsigned reverse_bits(unsigned x, int bits)
{
	x = ((Mask1 & x) <<  1) | ((x >>  1) & Mask1);
	x = ((Mask2 & x) <<  2) | ((x >>  2) & Mask2);
	x = ((Mask3 & x) <<  4) | ((x >>  4) & Mask3);
	x = ((Mask4 & x) <<  8) | ((x >>  8) & Mask4);
	x = ((Mask5 & x) << 16) | ((x >> 16) & Mask5);

	return x >> (32 - bits);
}

using Complex = std::complex<double>;
using Samples = std::vector<Complex>;

void fft(Samples &X, int bits, bool reverse = false)
{
	for(int i = 0; i < (1 << bits); ++i)
	{
		int j = reverse_bits(i, bits);
		if(i < j) std::swap(X[i], X[j]);
	}

	for(int b = 0; b < bits; ++b)
	{
		double angle = M_PI / (1 << b);
		Complex root = {cos(angle), sin(angle) * (reverse ? -1 : 1)};

		for(unsigned i = 0; i < X.size(); i += (2 << b))
		{
			Complex w {1, 0};
			for(unsigned j = i; j < i + (1 << b); ++j)
			{
				X[j + (1 << b)] *= w;
				auto temp = X[j] - X[j + (1 << b)];
				X[j] += X[j + (1 << b)];
				X[j + (1 << b)] = temp;

				w *= root;
			}
		}
	}
}

int main()
{
	std::cin.tie(0);
	std::ios::sync_with_stdio(0);

	std::string num1, num2;
	std::cin >> num1 >> num2;
	
	int bits = 32 - __builtin_clz(num1.size() + num2.size() - 1);
	int n = 1 << bits;

	Samples samples1(n), samples2(n);

	for(unsigned i = 0; i < num1.size(); ++i)
		samples1[i] = (double)(num1[num1.size() - 1 - i] - '0');
	fft(samples1, bits);

	for(unsigned i = 0; i < num2.size(); ++i)
		samples2[i] = (double)(num2[num2.size() - 1 - i] - '0');
	fft(samples2, bits);

	Samples product(n);
	for(int i = 0; i < n; ++i)
		product[i] = samples1[i] * samples2[i];

	fft(product, bits, true);

	std::vector<int> answer(n);
	for(int i = 0; i < n; ++i)
		answer[i] = round(product[i].real() / n);
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
