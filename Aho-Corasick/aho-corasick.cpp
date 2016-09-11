#include<iostream>
#include<string>
#include<queue>
#include<vector>

const int ALPHABET_SIZE = 26;
const int CHAR_OFFSET = 'a';

struct trie {
	trie* letter[ALPHABET_SIZE];
	trie* fail_link;
	trie* success_link;
	int index;

	trie()
	{
		for(auto &x : letter) x = nullptr;
		fail_link = nullptr;
		success_link = nullptr;
		index = -1;
	}
};

void push(trie *x, const std::string& s, int index)
{
	for(char c : s)
	{
		if(x->letter[c - CHAR_OFFSET] == nullptr)
			x->letter[c - CHAR_OFFSET] = new trie;
		x = x->letter[c - CHAR_OFFSET];
	}
	x->index = index;
}

void precompute(trie *root)
{
	std::queue<trie*> q;
	q.push(root);

	while(!q.empty())
	{
		trie *x = q.front(); q.pop();

		{ // compute success link
			trie *&y = x->success_link;
			y = x->fail_link;
			while(y != nullptr && y->index < 0)
				y = y->fail_link;
		}

		for(int i = 0; i < ALPHABET_SIZE; ++i)
		{
			if(x->letter[i] != nullptr)
			{
				trie *y = x->fail_link;
				while(y != nullptr && y->letter[i] == nullptr)
					y = y->fail_link;
				x->letter[i]->fail_link = y == nullptr ? root : y->letter[i];

				q.push(x->letter[i]);
			}
		}
	}
}

void print_match(const std::string& p, int index)
{
	while(index--) std::cout << ' ';
	std::cout << p << '\n';
}

int main()
{
	int pattern_count;
	std::cin >> pattern_count;
	std::vector<std::string> patterns(pattern_count);

	trie root;

	for(int i = 0; i < pattern_count; ++i)
	{
		std::cin >> patterns[i];
		push(&root, patterns[i], i);
	}

	precompute(&root);

	std::string text;
	std::cin >> text;

	{ // search
		trie *x = &root;

		for(unsigned i = 0; i < text.size(); ++i)
		{
			while(x != nullptr && x->letter[text[i] - CHAR_OFFSET] == nullptr)
				x = x->fail_link;

			if(x == nullptr)
			{
				x = &root;
			}
			else
			{
				x = x->letter[text[i] - CHAR_OFFSET];

				if(x->index >= 0)
					print_match(patterns[x->index], i + 1 - patterns[x->index].size());
				for(trie *y = x->success_link; y != nullptr; y = y->success_link)
					print_match(patterns[y->index], i + 1 - patterns[y->index].size());
			}
		}
	}
}
