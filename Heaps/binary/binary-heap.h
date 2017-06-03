#include<vector>

template<class T>
class BinaryHeap {
	private:
		std::vector<T> elements;

	public:
		inline bool empty();
		inline unsigned size();
		void push(const T&);
		void pop();
		T top();
};

template<class T>
inline bool BinaryHeap<T>::empty() {
	return size() == 0;
}

template<class T>
inline unsigned BinaryHeap<T>::size() {
	return elements.size();
}

template<class T>
void BinaryHeap<T>::push(const T& x) {
	auto ind = elements.size();
	elements.push_back(x);
	while(ind > 0 && x < elements[(ind - 1) / 2]) {
		std::swap(elements[ind], elements[(ind - 1) / 2]);
		ind = (ind - 1) / 2;
	}
}

template<class T>
T BinaryHeap<T>::top() {
	return elements[0];
}

template<class T>
void BinaryHeap<T>::pop() {
	elements[0] = elements.back();
	elements.pop_back();

	unsigned ind = 0;

	while(ind * 2 + 2 < elements.size()) {
		unsigned new_ind = ind * 2;
		new_ind += elements[new_ind + 1] < elements[new_ind + 2] ? 1 : 2;
		if(elements[new_ind] < elements[ind]) {
			std::swap(elements[ind], elements[new_ind]);
			ind = new_ind;
		}
		else break;
	}
	
	if(ind * 2 + 1 < elements.size() && elements[ind * 2 + 1] < elements[ind]) {
		std::swap(elements[ind], elements[ind * 2 + 1]);
	}
}
