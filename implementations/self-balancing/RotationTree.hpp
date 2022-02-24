#ifndef _STDLIB_ROTATIONTREE_HPP
#define _STDLIB_ROTATIONTREE_HPP

#include <BST>

namespace std {
	template<typename T, typename _node=_regular_bintree_node<T>, typename Compare=less<T>> class RotationTree : public BST<T, _node, Compare> {
	private:
		typedef _node node;

		/*
			Conditional functions.
			left_rotation and right_rotation are almost identical.
			In order to avoid writing the same code twice, I take out of my sleeve
			these two functions. They are implemented as a template for obtaining
			the same speed as the code was written twice.
			Both receive a template parameter "lr" (left/right). Being false=left, true=right.
			If lr is false (left):
				cprune(tree, n, branch) = tree.prune_left(n, branch)
				cgraft(tree, n, branch) = tree.graft_left(n, branch)
			If lr is true (right):
				cprune(tree, n, branch) = tree.prune_right(n, branch)
				cgraft(tree, n, branch) = tree.graft_right(n, branch)
		*/

		template<bool lr> inline void cprune(bintree<T, node>& tree, node n, bintree<T, node>& branch) {
			if(!lr)
				tree.prune_left(n, branch);
			else
				tree.prune_right(n, branch);
		}

		template<bool lr> inline void cgraft(bintree<T, node>& tree, node n, bintree<T, node>& branch) {
			if(!lr)
				tree.graft_left(n, branch);
			else
				tree.graft_right(n, branch);
		}

		/*
			The real rotation.
			I write the comments as if lr = false. That is, for a left rotation,
			but behavior depends on lr.
		*/
		template<bool lr> void rotate(node n) {
			bintree<T, node> aux, aux2;

			// [0] Cut the left child of n's right.
			if(!lr)
				cprune<lr>(this->data, n.right(), aux);
			else
				cprune<lr>(this->data, n.left(), aux);

			// [1] Cut n's right.
			cprune<!lr>(this->data, n, aux2);

			// [2] n's right will be the left of n's right.
			cgraft<!lr>(this->data, n, aux);

			// [3] The left of n's right will be its parent.
			// The following steps depend on whether n is the root or not.
			if(n.parent().null()) {
				// Root.
				// [3.1] Current root will be left child (hence LEFT rotation).
				if(!lr)
					aux2.root().left(this->data.root());
				else
					aux2.root().right(this->data.root());
				this->data.root().parent(aux2.root());
				// [3.2] New root is n's right.
				this->data.root(aux2.root());
				aux2.root(node());
			} else {
				// Totally not the root. Is n at the left or the right of its parent?
				node parent = n.parent();
				bool left = (parent.left() == n);
				if(left) {
					this->data.prune_left(parent, aux);
					cgraft<lr>(aux2, aux2.root(), aux);
					this->data.graft_left(parent, aux2);
				} else {
					this->data.prune_right(parent, aux);
					cgraft<lr>(aux2, aux2.root(), aux);
					this->data.graft_right(parent, aux2);
				}
			}
		}

	protected:
		// Assumes n has a right child.
		inline void left_rotation(node n) {
			rotate<false>(n);
		}

		// Assumes n has a left child.
		inline void right_rotation(node n) {
			rotate<true>(n);
		}
	};
}

#endif
