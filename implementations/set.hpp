#ifndef SET_HPP
#define SET_HPP

#include <AVL>

template<typename T, typename Compare=less<T>> class set : public AVL<T, Compare> {
public:
	using AVL<T, Compare>::AVL;
};

#endif
