#ifndef _STDLIB_UNORDERED_SET_HPP
#define _STDLIB_UNORDERED_SET_HPP

/*#include <RHHT>

namespace std {
	template<typename T>
	using unordered_set = RHHT<T>;
}*/

#include <set>
namespace std {
	template<typename T> using unordered_set = set<T>;
};

#endif
