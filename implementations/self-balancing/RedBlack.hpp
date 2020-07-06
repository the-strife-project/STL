#ifndef REDBLACK_HPP
#define REDBLACK_HPP

#include <implementations/self-balancing/RedBlack_node.hpp>
#include <implementations/self-balancing/RotationTree.hpp>

/*
	THIS DOESN'T WORK AND I HAVE ABSOLUTELY NO IDEA WHY

	"RED ROOT WTF" gets printed, even though
	"???" doesn't. I'm assuming it's not a problem
	with the RedBlack itself, but it's related to something else.

	By now, "set" and "map" rely on AVL, which works.
*/

template<typename T, typename Compare=less<T>> class RedBlack : public RotationTree<T, _RedBlack_node<T>, Compare> {
protected:
	typedef _RedBlack_node<T> node;

	const static bool BLACK = false;
	const static bool RED = true;

	node _insert(const T& e) override {
		printf("[RBI, %d]", e);
		// Podría dumpear aquí el árbol.

		node n = RotationTree<T, node, Compare>::_insert(e);
		node x = n;

		// Already inserted?
		if(n.null())
			return n;

		if(x.parent().null())
			x.color(BLACK);

		if(this->data.root().red()) {
			printf("RED ROOT WTF\n");
			while(true);
		}

		while(x.parent().red()) {
			node u = x.uncle();
			if(u.red()) {
				x.parent().color(BLACK);
				u.color(BLACK);
				x = x.parent().parent();
				if(!x.parent().null())
					x.color(RED);
			} else {
				// Triangle cases. Will fall back to the others.
				if(x == x.parent().right() && x.parent() == x.parent().parent().left()) {
					// Triangle pointing left.
					RotationTree<T, node, Compare>::left_rotation(x.parent());
					x = x.left();
				} else if(x == x.parent().left() && x.parent() == x.parent().parent().right()) {
					// Triangle pointing right.
					RotationTree<T, node, Compare>::right_rotation(x.parent());
					x = x.right();
				}

				// Straight line cases.
				if(x == x.parent().left() && x.parent() == x.parent().parent().left()) {
					// Left straight.
					auto aux = x.parent().color();
					x.parent().color(x.parent().parent().color());
					x.parent().parent().color(aux);
					RotationTree<T, node, Compare>::right_rotation(x.parent().parent());
				} else if(x == x.parent().right() && x.parent() == x.parent().parent().right()) {
					auto aux = x.parent().color();
					x.parent().color(x.parent().parent().color());
					x.parent().parent().color(aux);
					RotationTree<T, node, Compare>::left_rotation(x.parent().parent());
				}

				break;
			}
		}

		if(this->data.root().red()) {
			printf("???");
			while(true);
		}

		return n;
	}

	void _erase(node n) override {
		printf("[RBD, %d]", *n);while(true);
		if(this->data.root().red()) {
			printf("what %d.\n", *(this->data.root()));
			while(true);
		}

		if(n.left().null() && n.right().null()) {
			printf("A\n");
			// Leaf.
			if(n.red()) {
				if(this->size() == 1) printf("fuck\n");
			} else {
				// n would be left after erasion as a double black.
				fixDoubleBlack(n);
			}

			BST<T, node, Compare>::_erase(n);
		} else if(n.left().null() || n.right().null()){
			// One child.
			printf("it's going to shit itself\n");
			node child = !n.left().null() ? n.left() : n.right();
			if(n.parent().null()) {
				// Root.
				child.color(BLACK);
			} else {
				if(n.red() || child.red()) {
					child.color(BLACK);
				} else {
					// n would be left as a double black.
					fixDoubleBlack(n);
				}
			}

			BST<T, node, Compare>::_erase(n);
		} else if(!n.left().null() && !n.right().null()) {
			printf("C\n");
			// Two children.
			node succ = n.right();
			while(!succ.left().null())
				succ = succ.left();

			// Actual swap (not just the values) so iterators are not invalidated.
			BST<T, node, Compare>::_swap(n, succ);

			auto aux = n.color();
			n.color(succ.color());
			succ.color(aux);
			_erase(n);
		}

		if(this->data.root().red()) {
			printf("WTF!!!!\n");
			while(true);
		}
	}

	/*
		I would like to take a moment to give thanks to the following video:
		https://www.youtube.com/watch?v=CTvfzU_uNKE
	*/
	void fixDoubleBlack(node n) {
		printf("fixDoubleBlack.\n");
		if(this->data.root().red()) {
			printf("???...");
			while(true);
		}

		while(true) {
			// Root?
			if(n.parent().null()) {
				n.color(BLACK);
				break;	// Terminal case.
			}

			node s = n.sibling();

			// Red sibling?
			if(s.red()) {
				n.parent().color(RED);
				s.color(BLACK);
				if(s.parent().left() == s)
					RotationTree<T, node, Compare>::right_rotation(n.parent());
				else
					RotationTree<T, node, Compare>::left_rotation(n.parent());
			} else {
				// Black sibling. Children?
				if(s.left().black() && s.right().black()) {
					// Two black children. Parent?
					if(n.parent().black()) {
						// Black parent.
						s.color(RED);
						n = n.parent();	// Pushed the problem upwards.
					} else {
						// Red parent.
						s.color(RED);
						s.parent().color(BLACK);
						break;	// Terminal case.
					}
				} else {
					// Is the right child of sibling black?
					if(s.right().black()) {
						s.color(RED);
						s.left().color(RED);
						RotationTree<T, node, Compare>::right_rotation(s);
					} else {
						// Right child is red.
						s.right().color(BLACK);
						s.color(n.parent().color());
						n.parent().color(BLACK);
						RotationTree<T, node, Compare>::left_rotation(s);
						break;	// Terminal case.
					}
				}
			}
		}

		if(this->data.root().red()) {
			printf("I don't get it...");
			while(true);
		}
	}
};

#endif
