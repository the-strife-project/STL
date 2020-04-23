#ifndef PAIR_H
#define PAIR_H

template<typename T, typename Q> struct pair {
	T f;
	Q s;

	pair() = default;
	pair(const T& t, const Q& q)
		: f(t), s(q)
	{}

	bool operator<(const pair& other) {
		return (f < other.f);
	}
};

#endif
