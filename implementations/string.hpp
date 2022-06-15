#ifndef _STDLIB_STRING_HPP
#define _STDLIB_STRING_HPP

#include <vector>
#include <list>
#include <functional>

namespace std {
	class string : public vector<char> {
	public:
		using vector<char>::vector;

		string(const char* other) {
			*this = other;
		}

		inline size_t length() const {
			return vector<char>::size();
		}

		bool isNatural() const {
			for(auto x : *this)
				if(x < '0' || x > '9')
					return false;
			return true;
		}

		inline const char* c_str() {
			// Here's the idea: push and pop. The zero will be left there. Pop never frees.
			*this += 0;
			this->pop_back();
			return this->data;
		}

		inline void append(const string& other) {
			*this += other;
		}

		inline void appendb(const string& other) {
			vector<char>::push_front(other);
		}

		string& operator=(const char* other) {
			vector<char>::clear();

			for(size_t i=0; other[i]; ++i)
				vector<char>::push_back(other[i]);
			return *this;
		}

		inline string operator+(const string& other) const {
			string ret(*this);
			ret += other;
			return ret;
		}

		inline string& operator+=(const string& other) {
			vector<char>::push_back(other);
			return *this;
		}

		inline string& operator+=(char c) {
			vector<char>::push_back(c);
			return *this;
		}

		vector<string> split(char c) const {
			vector<string> ret;

			string aux;
			auto it = begin();
			do {
				if(it == end() || *it == c) {
					ret.push_back(aux);
					aux.clear();
				} else {
					aux += *it;
				}
			} while((it++) != end());

			return ret;
		}

		inline bool operator==(const string& other) const {
			return vector<char>::operator==(other);
		}

		inline bool operator==(const char* other) const {
			return (*this == string(other));
		}

		inline bool operator!=(const char* other) const {
			return !(*this == other);
		}

		bool operator<(const string& other) const {
			if(vector<char>::size() < other.size())
				return true;
			else if(vector<char>::size() > other.size())
				return false;

			for(size_t i=0; i<vector<char>::size(); ++i) {
				if((*this)[i] < other[i])
					return true;
				else if((*this)[i] > other[i])
					return false;
			}

			return false;
		}

		bool operator>(const string& other) const {
			if(vector<char>::size() > other.size())
				return true;
			else if(vector<char>::size() < other.size())
				return false;

			for(size_t i=0; i<vector<char>::size(); ++i) {
				if((*this)[i] > other[i])
					return true;
				else if((*this)[i] < other[i])
					return false;
			}

			return false;
		}

		// I truly don't know how to make this work in the ecosystem
		// Templates don't seem to work
		// I've just given up
		static const size_t hash_p = 31;
		static const size_t hash_m = 1000000009;
		inline virtual Hash hash() const {
			// Polynomial rolling hash function
			size_t ret = 0;
			size_t ppow = 1;
			for(char c : *this) {
				ret = (ret + c * ppow) % hash_m;
				ppow = (ppow * hash_p) % hash_m;
			}
			return ret;
		}
	};

	template<> struct hash<string> {
		inline Hash operator()(const string& x) const noexcept {
			return x.hash();
		}
	};
}

#endif
