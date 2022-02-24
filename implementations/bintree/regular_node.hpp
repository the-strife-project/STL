#ifndef _STDLIB_BINTREE_REGULAR_NODE
#define _STDLIB_BINTREE_REGULAR_NODE

/*
	This defines a regular bintree node.
	That is, one for use in, for instance, the AVL.
	Red-Black requires another one.

	Any other node must be implemented similar to this,
	giving friend access to bintree, etc.
*/

namespace std {
	template<typename T> class _regular_bintree_node {
	private:
		typedef _regular_bintree_node node;

		struct realnode {
			T tag;
			node parent;
			node left;
			node right;

			realnode() {}
			realnode(const T& tag)
				: tag(tag)
			{}
		};

		realnode* data;

	public:
		_regular_bintree_node()
			: data(nullptr)
		{}

		_regular_bintree_node(const T& tag) { data = new realnode(tag); }
		_regular_bintree_node(const node& other)
			: data(other.data)
		{}

		inline bool null() const { return data == nullptr; }
		inline node parent() const { return data->parent; }
		inline void parent(node n) { data->parent = n; }
		inline node left() const { return data->left; }
		inline void left(node n) { data->left = n; }
		inline node right() const { return data->right; }
		inline void right(node n) { data->right = n; }

		inline void destroy() {
			delete data;
			data = nullptr;
		}

		inline T& operator*() { return data->tag; }
		inline const T& operator*() const { return data->tag; }

		inline node& operator=(const node& other) {
			data = other.data;
			return *this;
		}

		inline bool operator==(const node& other) const {
			return data == other.data;
		}

		inline bool operator!=(const node& other) const {
			return data != other.data;
		}
	};
}

#endif
