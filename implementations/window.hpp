#ifndef WINDOW_H
#define WINDOW_H

#include <klibc/STL/iterable_stack>

template<typename T> class window {
private:
	iterable_stack<T> left_st, right_st;

public:
	window() = default;
	window(const window& other) = default;
	window(window&& other) = default;

	inline size_t size() const {
		return left_st.size() + right_st.size();
	}

	inline T& get() {
		return left_st.top();
	}

	inline const T& get() const {
		return left_st.top();
	}

	inline void put(const T& e) {
		left_st.push(e);
	}

	inline void remove() {
		left_st.pop();
	}

	inline void clear() {
		while(left_st.size()) left_st.pop();
		while(right_st.size()) right_st.pop();
	}

	inline void left() {
		right_st.push(left_st.top());
		left_st.pop();
	}

	inline void right() {
		left_st.push(right_st.top());
		right_st.pop();
	}

	inline size_t nleft() const {
		return left_st.size();
	}

	inline size_t nright() const {
		return right_st.size();
	}

	inline bool isBeginning() const {
		return left_st.size() == 0;
	}

	inline bool isFirst() const {
		return left_st.size() == 1;
	}

	inline bool isLast() const {
		return right_st.size() == 0;
	}

	window<T>& operator=(const window<T>& other) = default;
	window<T>& operator=(window<T>&& other) = default;

	window<T>& operator++() {
		right();
		return *this;
	}

	window<T> operator++(int) {
		window<T> ret = *this;
		right();
		return ret;
	}

	inline window<T>& operator--() {
		left();
		return *this;
	}

	inline window<T> operator--(int) {
		window<T> ret = *this;
		left();
		return ret;
	}

	typedef typename iterable_stack<T>::iterator iterator;
	typedef typename iterable_stack<T>::const_iterator const_iterator;

	/*
		begin() is not actually the beginning.
		The iterator of window is exactly the iterator of right_st.
		So, *begin() is right_st.top().
		Kind of weird, I know. It's what I need for the keyboard.
	*/
	inline iterator begin() {
		return right_st.begin();
	}

	inline iterator end() {
		return right_st.end();
	}

	inline const_iterator begin() const {
		return right_st.begin();
	}

	inline const_iterator end() const {
		return right_st.end();
	}
};

#endif
