#ifndef _STDLIB_HASH_HPP
#define _STDLIB_HASH_HPP

#include <types>

namespace std {
	typedef size_t Hash;

	// Primary class template hash
	template<typename T> struct hash;

	// Trivial hashes
	/*#define _STDLIB_TRIVIAL_HASH(x) \
	template<> struct hash<x> { \
		inline Hash operator()(
	};*/

	// Specialization for ints
	template<> struct hash<size_t> {
		inline Hash operator()(const size_t& x) const noexcept { return x; }
	};
}

#endif
