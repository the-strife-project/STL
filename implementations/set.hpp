#ifndef _STDLIB_SET_HPP
#define _STDLIB_SET_HPP

#include <AVL>

/*
	I'm aware "set" (ordered) would be slightly faster on insertion with a Red-Black.
	Originally, this was a RB. Implementation was faulty, resulting in red root.
	Apparently. Maybe. Or maybe it was a memory corruption from somewhere else.
	Any way, I decided the very slight improvement on insertion (with slower
	lookup) was not worth hours of my time, specially for the project I'm trying
	to achieve.

	I could implement it in the future, but I won't. There are way better things
	to invert my time. This is not, and will never be, the bottleneck of anything.
*/

namespace std {
	template<typename T, typename Compare=less<T>>
	class set : public AVL<T, Compare> {
	private:
		typedef AVL<T, Compare> inh;

	public:
		using inh::AVL;

		inline void insert(const T& t) {
			inh::_insert(t);
		}

		inline void add(const T& t) {
			inh::_insert(t);
		}
	};
}

#endif
