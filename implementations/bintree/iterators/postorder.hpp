class _template_postorder_iterator : public _template_iterator {
public:
	using _template_iterator::_template_iterator;

	_template_postorder_iterator& operator++() {
		if(this->cur.null())
			return *this;

		if(this->cur.parent().null()) {
			this->cur = node();
		} else {
			if(this->cur.parent().left() == this->cur) {
				if(!this->cur.parent().right().null()) {
					this->cur = this->cur.parent().right();
					do {
						while(!this->cur.left().null())
							this->cur = this->cur.left();
						if(!this->cur.right().null())
							this->cur = this->cur.right();
					} while(!this->cur.left().null() || !this->cur.right().null());
				} else {
					this->cur = this->cur.parent();
				}
			} else {
				this->cur = this->cur.parent();
			}
		}

		return *this;
	}

	_template_postorder_iterator operator++(int) {
		_template_postorder_iterator ret = *this;
		++(*this);
		return ret;
	}
};

class postorder_iterator : public _template_postorder_iterator {
public:
	using _template_postorder_iterator::_template_postorder_iterator;

	inline T& operator*() {
		return *(this->cur);
	}
};

inline postorder_iterator begin_postorder() {
	node n(_root);

	do {
		while(!n.left().null())
			n = n.left();
		if(!n.right().null())
			n = n.right();
	} while(!n.left().null() || !n.right().null());

	return n;
}

inline postorder_iterator end_postorder() { return node(); }

class const_postorder_iterator : public _template_postorder_iterator {
public:
	using _template_postorder_iterator::_template_postorder_iterator;

	inline const T& operator*() const {
		return *(this->cur);
	}
};

inline const_postorder_iterator begin_postorder() const { return _root; }
inline const_postorder_iterator end_postorder() const { return node(); }
