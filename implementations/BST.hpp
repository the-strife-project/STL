#ifndef BST_HPP
#define BST_HPP

#include <kernel/klibc/STL/bintree>
#include <kernel/klibc/STL/pair>

template<typename T, typename _node=_regular_bintree_node<T>> class BST {
protected:
	typedef _node node;

	bintree<T, node> data;
	size_t _size;

	/*
		Private function that tells us where an element should be.
		If it's in the tree, returns (true, node).
		Otherwise, returns (false, parent).
	*/
	pair<bool, node> _find(const T& e) const {
		if(data.empty())
			return pair<bool, node>(false, node());

		node parent = node();
		node n = data.root();
		while(!n.null()) {
			if(e == *n) {
				return pair<bool, node>(true, n);
			} else if(e < *n) {
				parent = n;
				n = n.left();
			} else {
				parent = n;
				n = n.right();
			}
		}

		return pair<bool, node>(false, parent);
	}

	/*
		Function to insert a value.
		Returns the inserted node.
	*/
	virtual node _insert(const T& e) {
		pair<bool, node> found = _find(e);

		node ret;
		if(found.s.null()) {
			// There's no root.
			data = e;
			ret = data.root();
		} else {
			if(found.f)
				return node();

			if(e < *(found.s)) {
				data.insert_left(found.s, e);
				ret = found.s.left();
			} else {
				data.insert_right(found.s, e);
				ret = found.s.right();
			}
		}

		++_size;
		return ret;
	}

	/*
		Function to delete a node.
		Returns its parent.
	*/
	virtual node _erase(node n) {
		/*
			Three scenarios:
			- Leaf (no children).
			- One child.
			- Two children.
		*/
		node parent = n.parent();
		bintree<T, node> aux;

		if(n.left().null() && n.right().null()) {
			// Leaf. Just remove it.
			if(parent.null())
				data.clear();
			else if(parent.left() == n)
				data.prune_left(parent, aux);
			else
				data.prune_right(parent, aux);
		} else if(n.left().null() || n.right().null()) {
			// One child. Substitute.
			if(parent.null()) {
				// Special case: it's the root.
				if(!n.left().null())
					data.prune_left(n, aux);
				else
					data.prune_right(n, aux);

				data.root(aux.root());
				aux.root(node());

				// n is now loose. Destroy it.
				n.destroy();

				--_size;
				return parent;
			}

			bool left = (parent.left() == n);
			if(left)
				data.prune_left(parent, aux);
			else
				data.prune_right(parent, aux);

			bintree<T, node> branch;
			if(!aux.root().left().null())
				aux.prune_left(aux.root(), branch);
			else
				aux.prune_right(aux.root(), branch);

			if(left)
				data.graft_left(parent, branch);
			else
				data.graft_right(parent, branch);
		} else {
			// Two children.
			// [0] Find the immediate next node (by value).
			node next = n.right();
			while(!next.left().null())
				next = next.left();

			// [1] next's left will be the one from n.
			data.prune_left(n, aux);
			data.graft_left(next, aux);

			// If the parent of next exists (it's not the root) and is not n...
			if(!next.parent().null() && next.parent() != n) {
				// [2] Drop the parent of next's left (pointer to next).
				next.parent().left(node());

				// [3] The parent of next's left will be next's right.
				data.prune_right(next, aux);
				data.graft_left(next.parent(), aux);

				// [4] next's right will be the n's right.
				data.prune_right(n, aux);
				data.graft_right(next, aux);
			}

			// [5] n's parent (left or right), if exists, now points to next.
			//     And the parent of next is now n's.
			if(!n.parent().null()) {
				if(n.parent().left() == n)
					n.parent().left(next);
				else
					n.parent().right(next);

				next.parent(n.parent());
			} else {
				// If it doesn't exist, n was the root.
				next.parent(node());
				data.root(next);
			}

			// [6] n is now loose. Destroy it.
			n.destroy();
		}

		--_size;
		return parent;
	}

public:
	// Iterators. Trivial.
	typedef typename bintree<T, node>::inorder_iterator iterator;
	typedef typename bintree<T, node>::const_inorder_iterator const_iterator;

	inline iterator begin() {
		return data.begin_inorder();
	}

	inline iterator end() {
		return data.end_inorder();
	}

	inline const_iterator begin() const {
		return data.begin_inorder();
	}

	inline const_iterator end() const {
		return data.begin_inorder();
	}


	// Methods.
	inline size_t size() const {
		return _size;
	}

	inline bool empty() const {
		return (_size == 0);
	}

	inline void clear() {
		data.clear();
		_size = 0;
	}

	inline iterator insert(const T& e) {
		return iterator(_insert(e));
	}

	iterator erase(iterator it) {
		iterator ret = it;
		++ret;

		_erase(it.getNode());
		return ret;
	}

	inline bool erase(const T& e) {
		pair<bool, node> found = _find(e);
		if(found.f)
			_erase(found.s);
		return found.f;
	}

	iterator find(const T& e) {
		pair<bool, node> found = _find(e);
		if(found.f)
			return iterator(found.s);
		else
			return iterator();
	}

	const_iterator find(const T& e) const {
		pair<bool, node> found = _find(e);
		if(found.f)
			return const_iterator(found.s);
		else
			return const_iterator();
	}
};

#endif
