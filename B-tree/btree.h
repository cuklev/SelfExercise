#include<algorithm>
#include<vector>
#include<optional>

template<typename T, unsigned D>
class BTree {
	private:
		struct MiddleNode;

		struct Node {
			std::vector<T> values;
			virtual ~Node() {}
		};

		using Leaf = Node;

		struct MiddleNode : Node {
			std::vector<Node*> children;
		};

		Node* root_;

		std::size_t size_;

	public:
		BTree()
			: root_(nullptr),
			size_(0) {
			}

		using branch = std::optional<std::pair<T, Node*>>;

	private:
		branch insert(Node* node, const T& value) {
			auto it = std::lower_bound(node->values.begin(), node->values.end(), value);
			if(it == node->values.end() || *it != value) {
				if(auto middle_node = dynamic_cast<MiddleNode*>(node)) {
					auto index = it - middle_node->values.begin();
					auto child_it = middle_node->children.begin() + index;
					if(auto b = insert(*child_it, value)) {
						middle_node->values.insert(std::move(it), b->first);
						middle_node->children.insert(middle_node->children.begin() + index + 1, b->second);
						if(middle_node->values.size() > D) {
							auto split = new MiddleNode;
							auto middle_index = D / 2;
							auto middle_it = middle_node->values.begin() + middle_index;
							auto middle_child_it = middle_node->children.begin() + middle_index + 1;

							std::move(middle_it + 1, middle_node->values.end(),
									std::back_inserter(split->values));
							std::move(middle_child_it, middle_node->children.end(),
									std::back_inserter(split->children));

							branch x = {{ std::move(*middle_it), split }};
							middle_node->values.resize(middle_index);
							middle_node->children.resize(middle_index);
							return x;
						}
					}
				} else { // Leaf
					node->values.insert(it, value);
					if(node->values.size() > D) {
						auto split = new Leaf;
						auto middle_index = D / 2;
						auto middle_it = node->values.begin() + middle_index;

						std::move(middle_it + 1, node->values.end(),
								std::back_inserter(split->values));

						branch x({ std::move(*middle_it), split });
						node->values.resize(middle_index);
						return x;
					}
				}
			}

			return {};
		}

	public:
		void insert(const T& value) {
			if(root_ == nullptr)
				root_ = new Leaf;
			if(auto b = insert(root_, value)) {
				auto new_root = new MiddleNode;
				new_root->values.push_back(std::move(b->first));
				new_root->children.push_back(root_);
				new_root->children.push_back(b->second);
				root_ = new_root;
			}
		}
};
