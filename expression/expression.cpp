#include<iostream>
#include<cmath>

using str = const std::string&;

double sum(str, uint32_t&);

double number(str s, uint32_t &index)
{
	if(s[index] == '(')
	{
		++index;
		double value = sum(s, index);
		if(s[index] != ')')
			throw "Expected ')'";
		++index;
		return value;
	}

	if(s[index] == '[')
	{
		++index;
		double value = std::floor(sum(s, index));
		if(s[index] != ']')
			throw "Expected ']'";
		++index;
		return value;
	}

	if(s[index] == '|')
	{
		++index;
		double value = std::abs(sum(s, index));
		if(s[index] != '|')
			throw "Expected '|'";
		++index;
		return value;
	}

	// TODO: Parse real numbers
	double value = 0;
	while(index < s.size())
	{
		if('0' <= s[index] && s[index] <= '9')
		{
			value = value * 10 + (s[index] - '0');
			++index;
		}
		else break;
	}

	return value;
}

double pow(str s, uint32_t &index)
{
	double value = number(s, index);

	if(index < s.size() && (s[index] == '^'))
	{
		++index;
		value = std::pow(value, pow(s, index));
	}

	return value;
}

double product(str s, uint32_t &index)
{
	double value = pow(s, index);

	while(index < s.size())
	{
		if(s[index] == '*')
		{
			++index;
			value *= pow(s, index);
		}
		else if(s[index] == '/')
		{
			++index;
			value /= pow(s, index);
		}
		else break;
	}

	return value;
}

double sum(str s, uint32_t &index)
{
	double value = product(s, index);

	while(index < s.size())
	{
		if(s[index] == '+')
		{
			++index;
			value += product(s, index);
		}
		else if(s[index] == '-')
		{
			++index;
			value -= product(s, index);
		}
		else break;
	}

	return value;
}

double evaluate(str s)
{
	uint32_t index = 0;
	return sum(s, index);
}

int main()
{
	std::string s;
	std::cin >> s;
	std::cout << evaluate(s) << '\n';
}
