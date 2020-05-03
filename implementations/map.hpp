#ifndef MAP_HPP
#define MAP_HPP

#include <kernel/klibc/STL/pair>
#include <kernel/klibc/STL/RedBlack>

template<typename T, typename Q, typename Compare=less<pair<T, Q>>> class map : public RedBlack<pair<T, Q>, Compare> {
private:
	typedef RedBlack<pair<T, Q>, Compare> inh;
public:
	using inh::RedBlack;

	inline Q& operator[](const T& t) {
		auto x = inh::_find(pair<T, Q>(t, Q()));
		if(x.f) {
			return (*(x.s)).s;
		} else {
			auto n = inh::_insert(pair<T, Q>(t, Q()));
			return (*n).s;
		}
	}
};

#endif
