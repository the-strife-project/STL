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

inline level_iterator begin_level() { return _root; }
inline level_iterator end_level() { return level_iterator(); }

class const_level_iterator : public _template_level_iterator {
public:
	using _template_level_iterator::_template_level_iterator;

	const T& operator*() const {
		return *(this->q.front());
	}
};

inline const_level_iterator begin_level() const { return _root; }
inline const_level_iterator end_level() const { return node(); }
