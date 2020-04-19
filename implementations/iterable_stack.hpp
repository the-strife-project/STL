#ifndef ITERABLE_STACK_H
#define ITERABLE_STACK_H

#include <kernel/klibc/STL/stack>

template<typename T> class iterable_stack : public stack<T> {
public:
	typedef typename list<T>::iterator iterator;
	typedef typename list<T>::const_iterator const_iterator;

	inline iterator begin() {
		return stack<T>::data.begin();
	}

	inline iterator end() {
		return stack<T>::data.end();
	}

	inline const_iterator begin() const {
		return stack<T>::data.begin();
	}

	inline const_iterator end() const {
		return stack<T>::data.end();
	}

	inline const_iterator cbegin() const {
		return stack<T>::data.cbegin();
	}

	inline const_iterator cend() const {
		return stack<T>::data.cend();
	}
};

#endif
