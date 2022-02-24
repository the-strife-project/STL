#ifndef _STDLIB_UNORDERED_SET_HPP
#define _STDLIB_UNORDERED_SET_HPP

// This is a meme. I'm aware.

#include <set>

namespace std {
	template<typename T, typename Compare=less<T>>
	using unordered_set = set<T, Compare>;
}

#endif
