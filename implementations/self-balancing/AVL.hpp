#ifndef _STDLIB_AVL_HPP
#define _STDLIB_AVL_HPP

#include <implementations/self-balancing/AVL_node.hpp>
#include <implementations/self-balancing/RotationTree.hpp>

inline int abs(int a) {
	return (a < 0) ? -a : a;
}

template<typename T, typename Compare=less<T>> class AVL : public RotationTree<T, _AVL_node<T>, Compare> {
protected:
	typedef _AVL_node<T> node;

	void ballance(node n) {
		while(!n.null()) {
			// [0] Update height.
			size_t left_height, right_height;
			pair<size_t, size_t> aux = n.recomputeHeight();
			left_height = aux.f;
			right_height = aux.s;

			// [1] Check for imbalance.
			if(abs(left_height - right_height) > 1) {
				// Rotation needed. Four cases. Where's the issue?

				if(left_height > right_height) {
					// At the left child's.
					aux = n.left().recomputeHeight();
					left_height = aux.f;
					right_height = aux.s;

					// If the issue is at the right, we must first rotate that.
					if(left_height < right_height) {
						// Left right rotation.
						RotationTree<T, node, Compare>::left_rotation(n.left());
						n.left().recomputeHeight();
						n.left().left().recomputeHeight();
					}

					// Right rotation on n.
					RotationTree<T, node, Compare>::right_rotation(n);
				} else {
					// At the right child's.
					aux = n.right().recomputeHeight();
					left_height = aux.f;
					right_height = aux.s;

					if(left_height > right_height) {
						// Right left rotation.
						RotationTree<T, node, Compare>::right_rotation(n.right());
						n.right().recomputeHeight();
						n.right().right().recomputeHeight();
					}

					// Left rotation on n.
					RotationTree<T, node, Compare>::left_rotation(n);
				}

				// Recompute heights of n and its parent.
				n.recomputeHeight();
				n.parent().recomputeHeight();
			}

			// Go to its parent.
			n = n.parent();
		}
	}

	inline node _insert(const T& e) override {
		node inserted = BST<T, node, Compare>::_insert(e);
		ballance(inserted.parent());
		return inserted;
	}

	inline void _erase(node n) override {
		node parent = n.parent();
		BST<T, node, Compare>::_erase(n);
		ballance(parent);
	}
};

#endif
