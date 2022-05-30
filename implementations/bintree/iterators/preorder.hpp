class _template_preorder_iterator : public _template_iterator {
public:
	using _template_iterator::_template_iterator;

	_template_preorder_iterator& operator++() {
		if(this->cur.null())
			return *this;

		if(!this->cur.left().null()) {
			this->cur = this->cur.left();
		} else if(!this->cur.right().null()) {
			this->cur = this->cur.right();
		} else {
			while((!this->cur.parent().null()) && (this->cur.parent().right() == this->cur || this->cur.parent().right().null()))
				this->cur = this->cur.parent();

			if(this->cur.parent().null())
				this->cur = node();
			else
				this->cur = this->cur.parent().right();
		}

		return *this;
	}

	_template_preorder_iterator operator++(int) {
		_template_preorder_iterator ret = *this;
		++(*this);
		return ret;
	}
};

class preorder_iterator : public _template_preorder_iterator {
public:
	using _template_preorder_iterator::_template_preorder_iterator;

	inline T& operator*() {
		return *(this->cur);
	}
};

inline preorder_iterator begin_preorder() { return _root; }
inline preorder_iterator end_preorder() { return node(); }

class const_preorder_iterator : public _template_preorder_iterator {
public:
	using _template_preorder_iterator::_template_preorder_tierator;

	inline const T& operator*() const {
		return *(this->cur);
	}
};

inline const_preorder_iterator begin_preorder() const { return _root; }
inline const_preorder_iterator end_preorder() const { return node(); }
