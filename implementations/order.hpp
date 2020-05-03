#ifndef ORDER_HPP
#define ORDER_HPP

template<typename T> struct less {
	bool operator()(const T& a, const T& b) const {
		return a < b;
	}
};

template<typename T> struct greater {
	bool operator()(const T& a, const T& b) const {
		return a > b;
	}
};

#endif
