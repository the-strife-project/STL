#ifndef PAIR_H
#define PAIR_H

template<typename T, typename Q> struct pair {
	T f;
	Q s;

	pair() = default;
	pair(const T& t, const Q& q)
		: f(t), s(q)
	{}

	inline bool operator<(const pair& other) const {
		return (f < other.f);
	}

	inline bool operator>(const pair& other) const {
		return (f > other.f);
	}

	inline bool operator==(const pair& other) const {
		return (f == other.f);
	}
};

#endif
