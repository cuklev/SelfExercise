#ifndef __AVL_TREE
#define __AVL_TREE

template<typename T>
class Avl {
	private:

		class Node;

		Node *root;

		bool insert(Node *&x, const T &value)
		{
			if(!x)
			{
				x = new Node(value);
				return true;
			}

			if(value < x->value)
				return insert(x->left, value);

			if(x->value < value)
				return insert(x->right, value);

			return false;
		}

		Node* get_leftmost(Node *&x)
		{
			if(!x->left)
			{
				auto y = x;
				x = x->right;
				return y;
			}

			return get_leftmost(x->left);
		}

		bool erase(Node *&x, const T &value)
		{
			if(!x)
				return false;

			if(value < x->value)
				return erase(x->left, value);

			if(x->value < value)
				return erase(x->right, value);

			if(!x->right)
			{
				auto y = x->left;
				delete x;
				x = y;
			}
			else
			{
				auto y = get_leftmost(x->right);
				y->left = x->left;
				y->right = x->right;
				delete x;
				x = y;
			}

			return true;
		}

		template<typename _>
		void foreach(Node *x, _ f)
		{
			if(!x) return;

			foreach(x->left, f);
			f(x->value);
			foreach(x->right, f);
		}

	public:

		Avl()
		{
			root = nullptr;
		}

		Node* find(const T &value)
		{
			auto x = root;
			
			while(x)
			{
				if(value < x->value)
					x = x->left;
				else if(x->value < value)
					x = x->right;
				else return x;
			}

			return nullptr;
		}

		bool insert(const T &value)
		{
			return insert(root, value);
		}

		bool erase(const T &value)
		{
			return erase(root, value);
		}

		template<typename _>
		void foreach(_ f)
		{
			foreach(root, f);
		}
};

template<typename T>
class Avl<T>::Node {
	public:

		T value;

		Node *left;
		Node *right;

		int size;
		int height;

		Node(const T &v)
		{
			value = v;
			left = nullptr;
			right = nullptr;

			size = 1;
			height = 1;
		}
};

#endif
