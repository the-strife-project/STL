#ifndef _STDLIB_SET_HPP
#define _STDLIB_SET_HPP

// Yes. This should be a RedBlack. But iirc implementation was faulty. TODO.

#include <AVL>

namespace std {
	template<typename T, typename Compare=less<T>> class set : public AVL<T, Compare> {
	public:
		using AVL<T, Compare>::AVL;
	};

	// This is a meme. I'm aware.
	template<typename T, typename Compare=less<T>>
	using unordered_set = set<T, Compare>;
}

#endif
