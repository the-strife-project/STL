#ifndef _STDLIB_BST_HPP
#define _STDLIB_BST_HPP

#include <bintree>
#include <pair>
#include <order>

namespace std {
	template<typename T, typename _node=_regular_bintree_node<T>, typename Compare=less<T>> class BST {
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
				} else if(Compare()(e, *n)) {
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

				if(Compare()(e, *(found.s))) {
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

		virtual void _erase(node n) {
			/*
				Three scenarios:
				- Leaf (no children).
				- One child.
				- Two children.
			*/
			node parent = n.parent();
			bintree<T, node> aux;

			if(n.left().null() && n.right().null()) {
				// Leaf. Just remove it. ret will be null.
				if(parent.null()) {
					data.clear();
				} else if(parent.left() == n) {
					parent.left(node());
					n.destroy();
				} else {
					parent.right(node());
					n.destroy();
				}
			} else if(n.left().null() || n.right().null()) {
				// One child. Substitute.
				if(parent.null()) {
					// Special case: it's the root.
					if(!n.left().null())
						data.root(n.left());
					else
						data.root(n.right());

					data.root().parent(node());
				} else {
					// It's not the root.
					if(!n.left().null()) {
						if(n.parent().left() == n)
							n.parent().left(n.left());
						else
							n.parent().right(n.left());
						n.left().parent(n.parent());
					} else {
						if(n.parent().left() == n)
							n.parent().left(n.right());
						else
							n.parent().right(n.right());
						n.right().parent(n.parent());
					}

					n.destroy();
				}
			} else {
				/*
					Two children.
					The usual case for this is to COPY next's tag to n and recurse.
					However, that invalidates all iterators. Which is not cool.
					What I am doing is swapping n and next, instead of copying, and
					then recurse.
				*/

				// Find the immediate next node (by value).
				node next = n.right();
				while(!next.left().null())
					next = next.left();

				_swap(n, next);
				parent = n.parent();

				// Recurse.
				_erase(n);
				++_size;	// Do not substract twice.
			}

			--_size;
		}

		// Swaps a node (a) with either its predecessor or its successor (b).
		void _swap(node a, node b) {
			// Swap collateral.
			if(!a.parent().null()) {
				if(a.parent().left() == a)
					a.parent().left(b);
				else
					a.parent().right(b);
			} else {
				data.root(b);
			}

			if(b.parent() != a) {
				if(b.parent().left() == b)
					b.parent().left(a);
				else
					b.parent().right(a);
			}

			if(!b.left().null())
				b.left().parent(a);
			if(!b.right().null())
				b.right().parent(a);
			if(!a.left().null() && a.left() != b)
				a.left().parent(b);
			if(!a.right().null() && a.right() != b)
				a.right().parent(b);

			// Swap.
			node p_aux = a.parent();
			if(b.parent() != a)
				a.parent(b.parent());
			else
				a.parent(b);
			b.parent(p_aux);

			node l_aux = a.left();
			a.left(b.left());
			if(l_aux != b)
				b.left(l_aux);
			else
				b.left(a);

			node r_aux = a.right();
			a.right(b.right());
			if(r_aux != b)
				b.right(r_aux);
			else
				b.right(a);
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
		BST()
			: _size(0)
		{}

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
}

#endif
