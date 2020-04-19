#ifndef QUEUE_H
#define QUEUE_H

#include <common/types.hpp>
#include <kernel/klibc/STL/list>

template<typename T> class queue {
private:
	list<T> data;

public:
	inline size_t size() const {
		return data.size();
	}

	inline void push(T e) {
		data.push_back(e);
	}

	inline T& front() {
		return data.front();
	}

	inline const T& front() const {
		return data.front();
	}

	inline T& back() {
		return data.back();
	}

	inline const T& back() const {
		return data.back();
	}

	inline void pop() {
		data.pop_front();
	}

	inline bool operator==(const queue<T>& other) const {
		return data == other.data;
	}

	inline bool operator!=(const queue<T>& other) const {
		return data != other.data;
	}
};

#endif
