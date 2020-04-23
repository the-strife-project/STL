#ifndef BST_HPP
#define BST_HPP

#include <kernel/klibc/STL/bintree>
#include <kernel/klibc/STL/pair>

template<typename T, typename _node=_regular_bintree_node<T>> class BST {
protected:
	typedef _node node;

	class tree : public bintree<T, node> {
	public:
		friend class BST<T, node>;
		using bintree<T, node>::bintree;
	};

	tree data;
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

	virtual void insert(const T& e) {
		pair<bool, node> found = _find(e);

		if(found.s.null()) {
			// There's no root.
			data = e;
		} else {
			if(found.f)
				return;

			if(e < *(found.s))
				data.insert_left(found.s, e);
			else
				data.insert_right(found.s, e);
		}

		++_size;
	}

	virtual void erase(node n) {
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
			if(parent.left() == n)
				data.prune_left(parent, aux);
			else
				data.prune_right(parent, aux);
		} else if(n.left().null() || n.right().null()) {
			// One child. Substitute.
			// TODO NOW: MAKE THIS SHORTER
			if(parent.left() == n) {
				// n is at the left of parent.
				data.prune_left(parent, aux);
				bintree<T, node> branch;
				if(!aux.root().left().null())
					branch.assign_subtree(aux, aux.root().left());
				else
					branch.assign_subtree(aux, aux.root().right());
				data.graft_left(parent, branch);
			} else {
				// n is at the right of parent.
				data.prune_right(parent, aux);
				bintree<T, node> branch;
				if(!aux.root().left().null())
					branch.assign_subtree(aux, aux.root().left());
				else
					branch.assign_subtree(aux, aux.root().right());
				data.graft_right(parent, branch);
			}
		} else {
			// Two children.

			// [0] Find the immediate next node (by value).
			node next = n.right();
			while(!next.left().null())
				next = next.left();

			// [1] next's left will be the one from n.
			data.prune_left(n, aux);
			data.graft_left(next, aux);

			// If the parent of next is not n...
			if(next.parent() != n) {
				// [2] Drop the parent of next's left (pointer to next).
				data.getRealnode(next.parent())->left = node();

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
					data.getRealnode(n.parent())->left = next;
				else
					data.getRealnode(n.parent())->right = next;

				data.getRealnode(next)->parent = n.parent();
			} else {
				// If it doesn't exist, n was the root.
				data.getRealnode(next)->parent = node();
				data._root = next;
			}

			// [6] n is now loose. Destroy it.
			n.remove();
		}

		--_size;
	}

	inline bool erase(const T& e) {
		pair<bool, node> found = _find(e);
		if(found.f)
			erase(found.s);
		return found.f;
	}

	iterator erase(iterator it) {
		iterator ret = it;
		++ret;

		erase(data.getCur(it));

		return ret;
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
