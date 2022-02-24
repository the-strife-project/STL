#ifndef _STDLIB_UNORDERED_MAP_HPP
#define _STDLIB_UNORDERED_MAP_HPP

// This is a meme. I'm aware.

#include <map>

template<typename T, typename Q, typename Compare=less<pair<T, Q>>>
using unordered_map = map<T, Compare>;

#endif
