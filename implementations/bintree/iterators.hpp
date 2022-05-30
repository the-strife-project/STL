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

#include "iterators/preorder.hpp"
#include "iterators/postorder.hpp"
#include "iterators/inorder.hpp"
#include "iterators/level.hpp"
