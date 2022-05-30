class _template_inorder_iterator : public _template_iterator {
public:
	using _template_iterator::_template_iterator;

	_template_inorder_iterator& operator++() {
		if(this->cur.null())
			return *this;

		if(!this->cur.right().null()) {
			this->cur = this->cur.right();
			while(!this->cur.left().null())
				this->cur = this->cur.left();
		} else {
			while(!this->cur.parent().null() && this->cur.parent().right() == this->cur)
				this->cur = this->cur.parent();

			this->cur = this->cur.parent();
		}

		return *this;
	}

	_template_inorder_iterator operator++(int) {
		_template_inorder_iterator ret = *this;
		++(*this);
		return ret;
	}
};

class inorder_iterator : public _template_inorder_iterator {
public:
	using _template_inorder_iterator::_template_inorder_iterator;

	T& operator*() {
		return *(this->cur);
	}
};

inline inorder_iterator begin_inorder() {
	node n(_root);

	// Just go as left as we can.
	if(!n.null())
		while(!n.left().null())
			n = n.left();

	return n;
}

inline inorder_iterator end_inorder() { return node(); }

class const_inorder_iterator : public _template_inorder_iterator {
public:
	using _template_inorder_iterator::_template_inorder_iterator;

	const T& operator*() const {
		return *(this->cur);
	}
};

inline const_inorder_iterator begin_inorder() const { return _root; }
inline const_inorder_iterator end_inorder() const { return node(); }
