/*
	This template implements a binary tree.
*/

#ifndef BINTREE_H
#define BINTREE_H

#include <common/types.hpp>
#include <kernel/klibc/STL/implementations/bintree/regular_node.hpp>

// Required for the level_iterator.
#include <kernel/klibc/STL/queue>

/*
	Regular bintree node.
	I make this as an external class so that the bintree receives
	a typename of the node, which would be _regular_bintree_node in most cases.
*/

template<typename T, typename _node=_regular_bintree_node<T>> class bintree {
public:
	typedef _node node;

private:
	typedef typename node::realnode realnode;

	node _root;

	// Private methods
	void destroy(node n) {
		if(!n.null()) {
			destroy(n.left());
			destroy(n.right());
			n.remove();
		}
	}

	void copy(node& dest, node& orig) {
		if(orig.null()) {
			dest = node();
			return;
		}

		// Step 1: copy the tag.
		dest = node(*orig);

		// Step 2: copy left.
		node aux(dest.left());
		copy(aux, orig.left());
		dest.left(aux);
		if(!dest.left().null())
			dest.left().parent(dest);

		// Step 3: copy right.
		aux = dest.right();
		copy(aux, orig.right());
		dest.right(aux);
		if(!dest.right().null())
			dest.right().parent(dest);
	}

	size_t count(node n) const {
		if(n.null())
			return 0;
		else
			return 1 + count(n.left()) + count(n.right());
	}

	bool equals(node n1, node n2) const {
		if(n1.null() && n2.null())
			return true;
		if(n1.null() || n2.null())
			return false;
		if(*n1 != *n2)
			return false;
		if(!equals(n1.left(), n2.left()))
			return false;
		if(!equals(n1.right(), n2.right()))
			return false;
		return true;
	}

public:
	bintree() = default;

	bintree(const T& root_tag)
		: _root(root_tag) {}

	bintree(const bintree<T>& other) {
		*this = other;
	}

	~bintree() {
		destroy(_root);
	}

	// Methods.
	inline node root() const {
		return _root;
	}

	inline size_t size() const {
		return count(_root);
	}

	inline bool empty() const {
		return (_root == node());
	}

	inline void clear() {
		destroy(_root);
		_root = node();
	}

	// Graft and insert.
	void graft_left(node n, bintree<T>& branch) {
		if(n.null()) return;

		destroy(node(n.left()));
		n.left(branch._root);
		if(!n.left().null())
			n.left().parent(n);
		branch._root = node();
	}

	inline void insert_left(node n, const T& tag) {
		bintree<T> aux(tag);
		graft_left(n, aux);
	}

	void graft_right(node n, bintree<T>& branch) {
		if(n.null()) return;

		destroy(node(n.right()));
		n.right(branch._root);
		if(!n.right().null())
			n.right().parent(n);
		branch._root = node();
	}

	inline void insert_right(node n, const T& tag) {
		bintree<T> aux(tag);
		graft_right(n, aux);
	}

	// Prune.
	void prune_left(node n, bintree<T>& orig) {
		if(n.null()) return;

		destroy(orig._root);
		orig._root = n.left();
		if(!orig._root.null())
			orig._root.parent(node());
		n.left(node());
	}

	void prune_right(node n, bintree<T>& orig) {
		if(n.null()) return;

		destroy(orig._root);
		orig._root = n.right();
		if(!orig._root().null())
			orig._root.parent(node());
		n.right(node());
	}

	void assign_subtree(const bintree<T>& tree, node n) {
		if(this != &tree) {
			// Not this tree.
			destroy(_root);
			copy(_root, n);
			if(!_root.null())
				_root.parent(node());
		} else {
			// This tree.
			if(_root != n) {
				node nod(_root);
				_root = n;
				if(!n.null()) {
					node aux(n.parent());
					if(n.parent().left() == n)
						aux.left(node());
					else
						aux.right(node());
				}
				destroy(nod);
				_root.parent(node());
			}
		}
	}

	// Operators.
	bintree<T>& operator=(const bintree<T>& other) {
		if(this != &other) {
			destroy(_root);
			copy(_root, other._root);
			if(!_root.null())
				_root.parent(node());
		}

		return *this;
	}

	inline bool operator==(const bintree<T>& other) const {
		return equals(_root, other._root);
	}

	inline bool operator!=(const bintree<T>& other) const {
		return !equals(_root, other._root);
	}

	// Iterators.
	#include <kernel/klibc/STL/implementations/bintree/iterators.hpp>
};

#endif
