#ifndef HEAP_HPP
#define HEAP_HPP

#include <bintree>
#include <order>

template<typename T, typename Compare=less<T>> class Heap {
private:
	bintree<T> data;
	size_t _size;

	typedef typename bintree<T>::node node;

	node nf;	// First non-full (in terms of children) node.
	node ne;	// Last non-empty node.

	// Next node in level.
	void goForward(node& n) {
		size_t height = 0;
		while(!n.parent().null() && n.parent().right() == n) {
			n = n.parent();
			++height;
		}

		if(!n.parent().null() && n.parent().left() == n) {
			n = n.parent();
			n = n.right();
		}

		bool rootReached = n.parent().null();

		while(height--)
			n = n.left();

		if(rootReached)
			n = n.left();
	}

	// Previous node in level.
	void goBack(node& n) {
		size_t height = 0;
		while(!n.parent().null() && n.parent().left() == n) {
			n = n.parent();
			++height;
		}

		if(!n.parent().null() && n.parent().right() == n) {
			n = n.parent();
			n = n.left();
		}

		if(n.parent().null())
			--height;

		while(!n.right().null() && height--)
			n = n.right();
	}

public:
	Heap()
		: _size(0)
	{}

	Heap(const Heap<T>& other) {
		*this = other;
	}

	Heap(Heap<T>&& other) {
		data = move(other.data);
		_size = other._size;
		nf = other.nf;
		ne = other.ne;

		other._size = 0;
		other.nf = node();
		other.ne = node();
	}

	~Heap() {
		data.clear();
		nf = node();
		ne = node();
	}

	Heap& operator=(const Heap<T>& other) {
		// TODO: Should search for nf and ne.
		if(this != &other) {
			data = other.data;
			_size = other._size;

			if(_size) {
				auto previous = data.begin_level();
				for(auto it=data.begin_level(); it!=data.end_level(); ++it) {
					if(it.getNode().left().null() || it.getNode().right().null()) {
						// Any of them is empty: not full.
						nf = it.getNode();
					}
					if(!(it.getNode().left().null() && it.getNode().right().null())) {
						// They are not null at the same time: not empty.
						ne = it.getNode();
					}
				}
			}
		}

		return *this;
	}

	Heap& operator=(Heap<T>&& other) {
		if(this != &other) {
			data = move(other.data);
			_size = other._size;
			nf = other.nf;
			ne = other.ne;

			other._size = 0;
			other.nf = node();
			other.ne = node();
		}

		return *this;
	}

	inline size_t size() const {
		return _size;
	}

	inline bool empty() const {
		return (_size == 0);
	}

	const T& top() const {
		return *(data.root());
	}

	void push(const T& e) {
		if(empty()) {
			data.root(e);
			nf = data.root();
			++_size;
			return;
		}

		auto n = nf;
		if(nf.left().null()) {
			data.insert_left(n, e);
			n = n.left();
			if(ne.null())
				ne = data.root();
			else
				goForward(ne);
		} else {
			data.insert_right(n, e);
			n = n.right();
			goForward(nf);
		}

		// Heapify.
		while(!n.parent().null() && Compare()(*n, *(n.parent()))) {
			T aux = *n;
			*n = *(n.parent());
			*(n.parent()) = aux;
			n = n.parent();
		}

		++_size;
	}

	void pop() {
		if(_size == 1) {
			data.clear();
			_size = 0;
			nf = node();
			return;
		}

		// Replace.
		bintree<T> garbage;
		if(ne.right().null()) {
			// Right is empty. Take left.
			*(data.root()) = *(ne.left());
			data.prune_left(ne, garbage);
			if(ne == data.root())
				ne = node();
			else
				goBack(ne);
		} else {
			*(data.root()) = *(ne.right());
			data.prune_right(ne, garbage);
			goBack(nf);
		}

		// Heapify.
		auto n = data.root();
		while(true) {
			node swap;
			if(n.left().null() && n.right().null())
				break;
			else if(n.left().null())
				swap = n.right();
			else if(n.right().null())
				swap = n.left();
			else
				swap = Compare()(*(n.left()), *(n.right())) ? n.left() : n.right();


			if(Compare()(*n, *swap))
				break;

			T aux = *n;
			*n = *swap;
			*swap = aux;
			n = swap;
		}

		--_size;
	}
};

#endif
