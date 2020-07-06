/*
	There are a lot of iterators defined in this file.
	I have tried to repeat as less code as possible.
*/

/*
	TEMPLATE ITERATOR

	This class is a template for the rest of iterators.
	Classes that inherit from this must implement:
	- operator*()
	- operator++()
	- operator++(int)
*/
class _template_iterator {
protected:
	node cur;

public:
	_template_iterator() = default;

	_template_iterator(node n)
		: cur(n)
	{}

	_template_iterator(const _template_iterator& other) = default;

	inline node getNode() const {
		return cur;
	}

	virtual _template_iterator& operator++() = 0;

	inline bool operator==(const _template_iterator& other) const {
		return (cur == other.cur);
	}

	inline bool operator!=(const _template_iterator& other) const {
		return (cur != other.cur);
	}

	inline _template_iterator& operator=(const _template_iterator& other) {
		cur = other.cur;
		return *this;
	}
};



// PREORDER
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

inline preorder_iterator begin_preorder() {
	return _root;
}

inline preorder_iterator end_preorder() {
	return node();
}

class const_preorder_iterator : public _template_preorder_iterator {
public:
	using _template_preorder_iterator::_template_preorder_tierator;

	inline const T& operator*() const {
		return *(this->cur);
	}
};

inline const_preorder_iterator begin_preorder() const {
	return _root;
}

inline const_preorder_iterator end_preorder() const {
	return node();
}


// POSTORDER
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

inline postorder_iterator end_postorder() {
	return node();
}

class const_postorder_iterator : public _template_postorder_iterator {
public:
	using _template_postorder_iterator::_template_postorder_iterator;

	inline const T& operator*() const {
		return *(this->cur);
	}
};

inline const_postorder_iterator begin_postorder() const {
	return _root;
}

inline const_postorder_iterator end_postorder() const {
	return node();
}


// INORDER
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

inline inorder_iterator end_inorder() {
	return node();
}

class const_inorder_iterator : public _template_inorder_iterator {
public:
	using _template_inorder_iterator::_template_inorder_iterator;

	const T& operator*() const {
		return *(this->cur);
	}
};

inline const_inorder_iterator begin_inorder() const {
	return _root;
}

inline const_inorder_iterator end_inorder() const {
	return node();
}


// LEVEL
class _template_level_iterator {
protected:
	queue<node> q;

public:
	_template_level_iterator() = default;
	_template_level_iterator(const _template_level_iterator& other) = default;

	_template_level_iterator(node n) {
		q.push(n);
	}

	inline bool operator!=(const _template_level_iterator& other) const {
		// I am NOT sure if this works in all cases.

		if(q.empty() && other.q.empty())
			return false;
		if(q.empty() || other.q.empty())
			return true;
		if(q.front() != other.q.front())
			return false;
		if(q.size() != other.q.size())
			return false;
		return (q == other.q);
	}

	inline bool operator==(const _template_level_iterator& other) const {
		return !(*this != other);
	}

	inline _template_level_iterator& operator=(const _template_level_iterator& other) {
		q = other.q;
		return *this;
	}

	_template_level_iterator& operator++() {
		if(!q.empty()) {
			node n = q.frontpop();
			if(!n.left().null())
				q.push(n.left());
			if(!n.right().null())
				q.push(n.right());
		}

		return *this;
	}

	_template_level_iterator operator++(int) {
		_template_level_iterator ret = *this;
		++(*this);
		return ret;
	}

	inline node getNode() const {
		return q.front();
	}
};

class level_iterator : public _template_level_iterator {
public:
	using _template_level_iterator::_template_level_iterator;

	T& operator*() {
		return *(this->q.front());
	}
};

inline level_iterator begin_level() {
	return _root;
}

inline level_iterator end_level() {
	return level_iterator();
}

class const_level_iterator : public _template_level_iterator {
public:
	using _template_level_iterator::_template_level_iterator;

	const T& operator*() const {
		return *(this->q.front());
	}
};

inline const_level_iterator begin_level() const {
	return _root;
}

inline const_level_iterator end_level() const {
	return node();
}
