#include<algorithm>
#include<vector>
#include<optional>

template<typename T, unsigned D>
class BTree {
	private:
		struct Node {
			std::vector<T> values;
			std::vector<Node*> children;

			Node() {
				values.reserve(D);
			}

			inline bool isLeaf() {
				return children.empty();
			}
		};

		Node* root_;

		std::size_t size_;

	public:
		BTree()
			: root_(nullptr),
			size_(0) {
			}

		class iterator;

	private:
		using Branch = std::optional<std::pair<T, Node*>>;
		using ResultType = std::pair<iterator, bool>;

	public:
		class iterator {
			private:
				using ValuePtr = std::pair<Node*, unsigned>;
				std::vector<ValuePtr> stack;

			public:
				inline T& operator*() {
					return stack.back().first->values[stack.back().second];
				}

				inline iterator& operator++() {
					++stack.back().second;
					while(!stack.back().first->isLeaf())
						stack.push_back({stack.back().first->children[stack.back().second], 0});
					while(stack.size() > 1 && stack.back().second == stack.back().first->values.size())
						stack.pop_back();
					return *this;
				}

				inline iterator& operator--() {
					while(!stack.back().first->isLeaf()) {
						auto child = stack.back().first->children[stack.back().second];
						stack.push_back({child, child->values.size()});
					}
					while(stack.size() > 1 && stack.back().second == 0)
						stack.pop_back();
					--stack.back().second;
					return *this;
				}

				inline bool operator!=(const iterator& other) {
					return stack.back() != other.stack.back();
				}
				inline bool operator==(const iterator& other) {
					return stack.back() == other.stack.back();
				}

				friend iterator BTree<T, D>::begin();
				friend iterator BTree<T, D>::end();
				friend Branch BTree<T, D>::insert(Node*, const T&, ResultType&);
				friend iterator BTree<T, D>::lower_bound(const T&);
		};

		iterator begin() {
			iterator it;
			it.stack.push_back({root_, -1});
			return ++it;
		}

		iterator end() {
			iterator it;
			it.stack.push_back({root_, root_ == nullptr ? 0 : root_->values.size()});
			return it;
		}

	private:
		Branch insert(Node* node, const T& value, ResultType& result) {
			auto it = std::lower_bound(node->values.begin(), node->values.end(), value);
			auto index = it - node->values.begin();
			result.first.stack.push_back({node, index});

			if(it == node->values.end() || *it != value) {
				if(node->isLeaf()) {
					node->values.insert(it, value);
					result.second = true;
					if(node->values.size() > D) {
						auto split = new Node;
						auto middle_index = D / 2;
						auto middle_it = node->values.begin() + middle_index;

						std::move(middle_it + 1, node->values.end(),
								std::back_inserter(split->values));

						Branch x({ std::move(*middle_it), split });
						node->values.resize(middle_index);
						return x;
					}
				} else {
					auto child_it = node->children.begin() + index;
					if(auto b = insert(*child_it, value, result)) {
						node->values.insert(std::move(it), b->first);
						node->children.insert(node->children.begin() + index + 1, b->second);
						if(node->values.size() > D) {
							auto split = new Node;
							split->children.reserve(D + 1);

							auto middle_index = D / 2;
							auto middle_it = node->values.begin() + middle_index;
							auto middle_child_it = node->children.begin() + middle_index + 1;

							std::move(middle_it + 1, node->values.end(),
									std::back_inserter(split->values));
							std::move(middle_child_it, node->children.end(),
									std::back_inserter(split->children));

							Branch x = {{ std::move(*middle_it), split }};
							node->values.resize(middle_index);
							node->children.resize(middle_index);
							return x;
						}
					}
				}
			}

			return {};
		}

	public:
		ResultType insert(const T& value) {
			ResultType result;
			if(root_ == nullptr)
				root_ = new Node;
			if(auto b = insert(root_, value, result)) {
				auto new_root = new Node;
				new_root->children.reserve(D + 1);
				new_root->values.push_back(std::move(b->first));
				new_root->children.push_back(root_);
				new_root->children.push_back(b->second);
				root_ = new_root;
			}
			if(result.second)
				++size_;
			return result;
		}

		iterator lower_bound(const T& value) {
			iterator result;
			if(root_ == nullptr)
				return end();

			Node* node = root_;
			while(true) {
				auto it = std::lower_bound(node->values.begin(), node->values.end(), value);
				auto index = it - node->values.begin();
				result.stack.push_back({node, index});
				if(it != node->values.end() && *it == value) break;
				if(node->isLeaf()) {
					while(result.stack.size() > 1 && result.stack.back().second == result.stack.back().first->values.size())
						result.stack.pop_back();
					break;
				} else {
					node = node->children[index];
				}
			}

			return result;
		}

		iterator find(const T& value) {
			auto it = lower_bound(value);
			return *it == value ? it : end();
		}
};
