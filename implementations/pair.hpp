#ifndef _STDLIB_PAIR_H
#define _STDLIB_PAIR_H

// Shouldn't this have a Compare?

#include <functional>

namespace std {
	template<typename T, typename Q> struct pair {
		T f;
		Q s;

		pair() = default;
		pair(const T& t, const Q& q)
			: f(t), s(q)
		{}

		inline bool operator<(const pair& other) const {
			return f < other.f;
		}

		inline bool operator>(const pair& other) const {
			return f > other.f;
		}

		inline bool operator==(const pair& other) const {
			return f == other.f;
		}

		inline Hash hash() const { return std::hash<T>()(f); }
	};

	template<typename T, typename Q>
	struct hash<pair<T, Q>> {
		inline Hash operator()(const pair<T, Q>& x) const noexcept {
			return x.hash();
		}
	};
}

#endif
