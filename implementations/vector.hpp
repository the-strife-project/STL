#ifndef _STDLIB_VECTOR_H
#define _STDLIB_VECTOR_H

#include <types>
#include <cstring>
#include <algorithm>

#define _STDLIB_VECTOR_INITIAL_SIZE 1

namespace std {
	template<typename T> class vector {
	protected:
		T* data = nullptr;
		size_t sz = 0;
		size_t allocated = 0;

		inline void more() {
			if(!allocated)
				reserve(_STDLIB_VECTOR_INITIAL_SIZE);
			else
				reserve(allocated * 2);
		}

	public:
		vector() = default;

		vector(const vector<T>& other) {
			sz = other.sz;
			allocated = other.allocated;

			data = new T[allocated];
			for(size_t i=0; i<sz; i++) data[i] = other.data[i];
		}

		vector(vector<T>&& other) {
			sz = other.sz;
			allocated = other.allocated;
			data = other.data;
			other.data = nullptr;
		}

		~vector() { clear(); }

		inline void reserve(size_t n) {
			allocated = n;

			T* newdata = new T[n];
			memmove(newdata, data, std::min(sz, n) * sizeof(T));

			if(data) delete [] data;
			data = newdata;
		}

		inline void resize(size_t n) {
			reserve(n);
			sz = n;
		}

		inline size_t size() const { return sz; }

		void disp_right(size_t idx, size_t count) {
			if(sz + count > allocated) {
				// We need to reallocate.
				// [[ This sucks a lot but I'm too lazy to make it better ]]
				allocated *= 2;

				T* newdata = new T[allocated];
				for(size_t i=0; i<idx; i++) newdata[i] = data[i];
				for(size_t i=idx+count; i<((sz-idx) + count); i++) newdata[i] = data[i];

				if(data) delete [] data;
				data = newdata;
			} else {
				// No need to reallocate.
				for(size_t i=sz+count-1; i>=idx+count; i--) data[i] = data[i-count];
			}

			sz += count;
		}

		void disp_left(size_t idx, size_t count) {
			for(size_t i=idx-count; i<sz-count; i++) data[i] = data[i+count];
			sz -= count;
		}

		// Back.
		void push_back(const T& e) {
			if(sz == allocated)
				more();
			data[sz++] = e;
		}

		void push_back(const vector<T>& other) {
			for(auto const& x : other)
				push_back(x);
		}

		inline void pop_back() { --sz; }
		inline T& back() { return data[sz-1]; }
		inline const T& back() const { return data[sz-1]; }

		// Front.
		inline void push_front(T e) {
			disp_right(0, 1);
			data[0] = e;
		}

		void push_front(const vector<T>& other) {
			disp_right(0, other.size());
			for(size_t i=0; i<other.size(); i++)
				data[i] = other.data[i];
		}

		inline void pop_front() { disp_left(1, 1); }
		inline T& front() { return data[0]; }
		inline const T& front() const { return data[0]; }

		// General operations.
		void invert() {
			size_t count = size() / 2;
			for(size_t i=0; i<count; i++) {
				T aux = data[i];
				data[i] = data[(size()-1)-i];
				data[(size()-1)-i] = aux;
			}
		}

		void clear() {
			if(data)
				delete [] data;
			data = nullptr;
			sz = allocated = 0;
		}

		// Operators.
		vector<T>& operator=(const vector<T>& other) {
			if(this != &other) {
				clear();

				sz = other.sz;
				allocated = other.allocated;

				data = new T[allocated];
				for(size_t i=0; i<sz; i++) data[i] = other.data[i];
			}

			return *this;
		}

		vector<T>& operator=(vector<T>&& other) {
			if(this != &other) {
				sz = other.sz;
				allocated = other.allocated;
				data = other.data;
				other.data = nullptr;
			}

			return *this;
		}

		inline T& operator[](size_t idx) { return data[idx]; }
		inline const T& operator[](size_t idx) const { return data[idx]; }

		bool operator==(const vector<T>& other) const {
			if(sz != other.sz) return false;
			for(size_t i=0; i<sz; i++)
				if(data[i] != other.data[i]) return false;

			return true;
		}

		inline bool operator!=(const vector<T>& other) const {
			return !(*this == other);
		}

		inline vector<T>& operator+=(const vector<T>& other) {
			push_back(other);
			return *this;
		}

		// Iterators.
		typedef T* iterator;
		typedef const T* const_iterator;

		inline iterator begin() { return data; }
		inline iterator end() { return data+sz; }
		inline const_iterator begin() const { return data; }
		inline const_iterator end() const { return data+sz; }
		inline const_iterator cbegin() const { return data; }
		inline const_iterator cend() const { return data+sz; }
	};
}

#endif
