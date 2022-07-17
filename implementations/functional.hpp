#ifndef _STDLIB_HASH_HPP
#define _STDLIB_HASH_HPP

#include <types>

namespace std {
	typedef size_t Hash;

	// Primary class template hash
	template<typename T> struct hash;

	// Specialization for ints
	template<> struct hash<uint8_t> {
		inline Hash operator()(const uint8_t& x) const noexcept { return x; }
	};
	template<> struct hash<uint16_t> {
		inline Hash operator()(const uint16_t& x) const noexcept { return x; }
	};
	template<> struct hash<uint32_t> {
		inline Hash operator()(const uint32_t& x) const noexcept { return x; }
	};
	template<> struct hash<uint64_t> {
		inline Hash operator()(const uint64_t& x) const noexcept { return x; }
	};
	template<> struct hash<int8_t> {
		inline Hash operator()(const int8_t& x) const noexcept { return x; }
	};
	template<> struct hash<int16_t> {
		inline Hash operator()(const int16_t& x) const noexcept { return x; }
	};
	template<> struct hash<int32_t> {
		inline Hash operator()(const int32_t& x) const noexcept { return x; }
	};
	template<> struct hash<int64_t> {
		inline Hash operator()(const int64_t& x) const noexcept { return x; }
	};
}

#endif
