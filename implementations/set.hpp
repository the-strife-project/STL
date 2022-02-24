#ifndef _STDLIB_SET_HPP
#define _STDLIB_SET_HPP

// Yes. This should be a RedBlack. But iirc implementation had faults. TODO.

#include <AVL>

namespace std {
	template<typename T, typename Compare=less<T>> class set : public AVL<T, Compare> {
	public:
		using AVL<T, Compare>::AVL;
	};
}

#endif
