#ifndef _STDLIB_MAP_HPP
#define _STDLIB_MAP_HPP

#include <pair>
#include <AVL>

namespace std {
	template<typename K, typename V, typename Compare=less<pair<K, V>>> class map : public AVL<pair<K, V>, Compare> {
	private:
		typedef AVL<pair<K, V>, Compare> inh;
	public:
		using inh::AVL;

		inline V& operator[](const K& t) {
			auto x = inh::_find(pair<K, V>(t, V()));
			if(x.f) {
				return (*(x.s)).s;
			} else {
				auto n = inh::_insert(pair<K, V>(t, V()));
				return (*n).s;
			}
		}

		inline bool has(const K& k) {
			auto x = inh::_find(pair<K, V>(k, V()));
			return x.f;
		}
	};
}

#endif
