#ifndef ALLOCATION_VECTOR_HPP
#define ALLOCATION_VECTOR_HPP

#include <implementations/vector.hpp>
#include <implementations/queue.hpp>

template<typename T> class AllocationVector {
private:
	vector<T> data;
	queue<size_t> freed;

public:
	inline size_t size() const {
		return data.size() - freed.size();
	}

	inline bool empty() const {
		return size() == 0;
	}

	size_t alloc() {
		if(freed.empty()) {
			data.push_back(T());
			return data.size()-1;
		} else {
			size_t ret = freed.front();
			freed.pop();
			return ret;
		}
	}

	inline void free(size_t idx) {
		freed.push(idx);
	}

	inline T& operator[](size_t idx) {
		return data[idx];
	}

	inline const T& operator[](size_t idx) const {
		return data[idx];
	}
};

#endif
