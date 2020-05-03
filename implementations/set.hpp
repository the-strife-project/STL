#ifndef SET_HPP
#define SET_HPP

#include <kernel/klibc/STL/RedBlack>

template<typename T, typename Compare=less<T>> class set : public RedBlack<T, Compare> {
public:
	using RedBlack<T, Compare>::RedBlack;
};

#endif
