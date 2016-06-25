#ifndef __AVL_TREE
#define __AVL_TREE

template<typename T>
class Avl {
	private:

		class Node;

		Node *root;

		int size(Node *x)
		{
			return x ? x->size : 0;
		}

		int height(Node *x)
		{
			return x ? x->height : 0;
		}

		void update(Node *x)
		{
			x->size = size(x->left) + size(x->right) + 1;
			int lh = height(x->left), rh = height(x->right);
			x->height = (lh > rh ? lh : rh) + 1;
		}

		int balance(Node *x)
		{
			return height(x->left) - height(x->right);
		}

		void rotateLeft(Node *&x)
		{
			auto y = x->right;
			x->right = y->left;
			update(x);
			y->left = x;
			x = y;
			update(x);
		}

		void rotateRight(Node *&x)
		{
			auto y = x->left;
			x->left = y->right;
			update(x);
			y->right = x;
			x = y;
			update(x);
		}

		void balanceLeft(Node *&x)
		{
			update(x);
			if(balance(x) > 1)
			{
				if(balance(x->left) < 0)
					rotateLeft(x->left);
				rotateRight(x);
			}
		}

		void balanceRight(Node *&x)
		{
			update(x);
			if(balance(x) < -1)
			{
				if(balance(x->right) > 0)
					rotateRight(x->right);
				rotateLeft(x);
			}
		}

		bool insert(Node *&x, const T &value)
		{
			if(!x)
			{
				x = new Node(value);
				return true;
			}

			if(value < x->value)
			{
				bool f = insert(x->left, value);
				balanceLeft(x);
				return f;
			}

			if(x->value < value)
			{
				bool f = insert(x->right, value);
				balanceRight(x);
				return f;
			}

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

			auto y = get_leftmost(x->left);
			balanceRight(x);
			return y;
		}

		bool erase(Node *&x, const T &value)
		{
			if(!x)
				return false;

			if(value < x->value)
			{
				bool f = erase(x->left, value);
				balanceRight(x);
				return f;
			}

			if(x->value < value)
			{
				bool f = erase(x->right, value);
				balanceLeft(x);
				return f;
			}

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
				balanceLeft(x);
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

		int size()
		{
			return size(root);
		}

		int height()
		{
			return height(root);
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
