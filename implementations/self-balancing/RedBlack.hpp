#ifndef REDBLACK_HPP
#define REDBLACK_HPP

#include <kernel/klibc/STL/implementations/self-balancing/RedBlack_node.hpp>
#include <kernel/klibc/STL/implementations/self-balancing/RotationTree.hpp>

template<typename T, typename Compare=less<T>> class RedBlack : public RotationTree<T, _RedBlack_node<T>, Compare> {
protected:
	typedef _RedBlack_node<T> node;

	const static bool BLACK = false;
	const static bool RED = true;

	node _insert(const T& e) override {
		node n = RotationTree<T, node, Compare>::_insert(e);
		if(n.null()) {
			// Already inserted!
			return n;
		}

		// Case 1: root? Color it black.
		if(n.parent().null()) {
			n.color(BLACK);
			return n;
		}

		// Case 2: not root. Leave it red.
		// (a) If its parent is black, we're done.
		if(n.parent().black())
			return n;

		// (b) Parent is red. This violates the «no red-red relationship» invariant. Let's fix it.
		node x = n;
		while(true) {
			// As this block is called several times, first we must make sure that
			// both the grandparent exists.
			node p = x.parent();
			if(p.null())
				break;

			node gp = p.parent();
			if(gp.null())
				break;

			if(x.uncle().null() || x.uncle().black()) {
				// Uncle is black or absent.
				// Rotate.
				if(p.left() == x && gp.left() == p) {
					// Straight line at the left.
					// Right rotation on grandparent.
					RotationTree<T, node, Compare>::right_rotation(gp);
					p.invertColor();
					gp.invertColor();
				} else if(p.right() == x && gp.right() == p) {
					// Straight line at the right.
					// Left rotation on grandparent.
					RotationTree<T, node, Compare>::left_rotation(gp);
					p.invertColor();
					gp.invertColor();
				} else if(p.left() == x && gp.right() == p) {
					// Triangle pointing to the right.
					// Right-left rotation.
					RotationTree<T, node, Compare>::right_rotation(p);
					x.invertColor();
					x.parent().invertColor();
					RotationTree<T, node, Compare>::left_rotation(gp);
				} else {
					// Triangle pointing to the left.
					// Left-right rotation.
					RotationTree<T, node, Compare>::left_rotation(p);
					x.invertColor();
					x.parent().invertColor();
					RotationTree<T, node, Compare>::right_rotation(gp);
				}

				// We've finished.
				break;
			} else {
				// Uncle is red. We must check everything up to the root.
				// First, parent and uncle turn black.
				p.color(BLACK);
				x.uncle().color(BLACK);
				// If grandparent is not the root, change its color to red.
				if(!gp.parent().null()) {
					gp.color(RED);
				} else {
					// Grandparent is the root. Leave it black. We're done.
					break;
				}

				x = gp;
				// Repeat!
			}
		}

		return n;
	}


	void _erase(node n) override {
		// Cases depend on the number of children of n, much like in BST deletion.
		if(n.left().null() && n.right().null()) {
			// Leaf.
			if(n.red()) {
				// Red leaf.
				if(!n.sibling().null())
					n.sibling().color(RED);
			} else {
				// Black leaf.
				// If deleting the root when it's on its own, this case will be called.
				// Fix, then delete.
				fixDoubleBlack(n);
				BST<T, node, Compare>::_erase(n);
			}
		} else if(n.left().null() || n.right().null()) {
			// One child.
			if(n.parent().null()) {
				// Root. Just delete it and make the next one black.
				if(!n.left().null())
					n.left().color(BLACK);
				else
					n.right().color(BLACK);

				BST<T, node, Compare>::_erase(n);
			} else {
				// Not the root.
				node child = !n.left().null() ? n.left() : n.right();
				bool eitherred = n.red() || child.red();
				BST<T, node, Compare>::_erase(n);
				if(eitherred) {
					// Color child black.
					child.color(BLACK);
				} else {
					// Both are black. Double black situation.
					fixDoubleBlack(child);
				}
			}
		} else {
			// Two children.
			// Get successor.
			node next = n.right();
			while(!next.left().null())
				next = next.left();
			// Swap.
			BST<T, node, Compare>::_swap(n, next);
			// Leave the colors unchanged.
			bool color_aux = n.color();
			n.color(next.color());
			next.color(color_aux);
			// Recurse and delete.
			_erase(n);
		}
	}

	void fixDoubleBlack(node n) {
		// While u is double black (break when necessary) and it's not the root...
		while(!n.parent().null()) {
			node s = n.sibling();

			if(s.null()) {
				// No sibling, double black pushed up.
				n = n.parent();
				continue;
			}

			if(s.black()) {
				// Sibling is black.
				// What about its children?
				if(s.left().red() || s.right().red()) {
					// At least one of the children is red.
					// Time to do some rotations.
					bool redchildpos = !s.left().red();	// False: left. True: right.
					node redchild = !redchildpos ? s.left() : s.right();

					if(!redchildpos) {
						// redchild is the left child of s.
						if(s.parent().left() == s) {
							redchild.color(s.color());
							s.color(n.parent().color());
							RotationTree<T, node, Compare>::right_rotation(n.parent());
						} else {
							redchild.color(n.parent().color());
							RotationTree<T, node, Compare>::right_rotation(s);
							RotationTree<T, node, Compare>::left_rotation(n.parent());
						}
					} else {
						// redchild is the right child of s.
						if(s.parent().left() == s) {
							redchild.color(n.parent().color());
							RotationTree<T, node, Compare>::left_rotation(s);
							RotationTree<T, node, Compare>::right_rotation(n.parent());
						} else {
							redchild.color(s.color());
							s.color(n.parent().color());
							RotationTree<T, node, Compare>::left_rotation(n.parent());
						}
					}

					n.parent().color(BLACK);
					break;
				} else {
					// Both of the children of the sibling are black.
					s.color(RED);
					if(n.parent().red()) {
						n.parent().color(BLACK);
						break;
					}
					// If the parent is black, push up double blackness.
					n = n.parent();
				}
			} else {
				// Sibling is red.
				n.parent().color(RED);
				s.color(BLACK);
				if(s.parent().left() == s) {
					// Sibling is at the left.
					RotationTree<T, node, Compare>::right_rotation(n.parent());
 				} else {
					// Sibling is at the right.
					RotationTree<T, node, Compare>::left_rotation(n.parent());
				}
			}
		}
	}
};

#endif
