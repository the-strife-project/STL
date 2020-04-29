#ifndef AVL_HPP
#define AVL_HPP

#include <kernel/klibc/STL/implementations/self-balancing/AVL_node.hpp>
#include <kernel/klibc/STL/implementations/self-balancing/RotationTree.hpp>

size_t max(size_t a, size_t b) {
	if(a > b)
		return a;
	else
		return b;
}

int abs(int a) {
	if(a < 0)
		a = -a;
	return a;
}

template<typename T> class AVL : public RotationTree<T, _AVL_node<T>> {
private:
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
						RotationTree<T, node>::left_rotation(n.left());
						n.left().recomputeHeight();
						n.left().left().recomputeHeight();
					}

					// Right rotation on n.
					RotationTree<T, node>::right_rotation(n);
				} else {
					// At the right child's.
					aux = n.right().recomputeHeight();
					left_height = aux.f;
					right_height = aux.s;

					if(left_height > right_height) {
						// Right left rotation.
						RotationTree<T, node>::right_rotation(n.right());
						n.right().recomputeHeight();
						n.right().right().recomputeHeight();
					}

					// Left rotation on n.
					RotationTree<T, node>::left_rotation(n);
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
		node inserted = BST<T, node>::_insert(e);
		ballance(inserted.parent());
		return inserted;
	}

	inline void _erase(node n) override {
		node parent = n.parent();
		BST<T, node>::_erase(n);
		ballance(parent);
	}
};

#endif
