#ifndef REDBLACKNODE_HPP
#define REDBLACKNODE_HPP

/*
	This defines a node for a Red-Black tree.
	The only difference with _regular_bintree_node is that
	this one saves the color.
	False = black
	True = red
*/

template<typename T> class _RedBlack_node {
private:
	typedef _RedBlack_node node;

	struct realnode {
		T tag;
		node parent;
		node left;
		node right;
		bool color;

		realnode() {}
		realnode(const T& tag)
			: tag(tag)
		{}
	};

	realnode* data;

public:
	_RedBlack_node()
		: data(nullptr)
	{}

	_RedBlack_node(const T& tag) {
		data = new realnode(tag);
		data->color = true;	// Default color is red.
	}

	_RedBlack_node(const node& other)
		: data(other.data)
	{}

	inline bool null() const {
		return (data == nullptr);
	}

	inline node parent() const {
		return data->parent;
	}

	inline void parent(node n) {
		data->parent = n;
	}

	inline node left() const {
		return data->left;
	}

	inline void left(node n) {
		data->left = n;
	}

	inline node right() const {
		return data->right;
	}

	inline void right(node n) {
		data->right = n;
	}

	/* --- Red-Black specifics --- */
	inline node uncle() const {
		if(parent().parent().left() == parent())
			return parent().parent().right();
		else
			return parent().parent().left();
	}

	inline node sibling() const {
		if(parent().left() == *this)
			return parent().right();
		else
			return parent().left();
	}

	inline bool color() const {
		// The color of a null node is black.
		if(!data)
			return false;
		else
			return data->color;
	}

	inline void color(bool x) {
		data->color = x;
	}

	inline void invertColor() {
		data->color = !(data->color);
	}

	// Neat shortcuts. Call color() in order to keep the null node protections.
	inline bool black() const {
		return !color();
	}

	inline bool red() const {
		return color();
	}
	/* --- End Red-Black specifics --- */

	inline void destroy() {
		delete data;
		data = nullptr;
	}

	inline T& operator*() {
		return data->tag;
	}

	inline const T& operator*() const {
		return data->tag;
	}

	inline node& operator=(const node& other) {
		data = other.data;
		return *this;
	}

	inline bool operator==(const node& other) const {
		return (data == other.data);
	}

	inline bool operator!=(const node& other) const {
		return (data != other.data);
	}
};

#endif
