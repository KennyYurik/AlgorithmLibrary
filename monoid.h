#pragma once

#include <limits>
typedef unsigned int uint;

template<typename T>
struct monoid_add {
	typedef T type;
	static const T zero;

	static T append(const T& a, const T& b) {
		return a + b;
	}

	static T pow(const T& arg, uint power) {
		return arg * power;
	}
};

template<typename T>
const T monoid_add<T>::zero = T(0);

template<typename T>
using monoid = monoid_add<T>;

template<typename T>
struct monoid_mul {
	typedef T type;
	static const T zero;

	static T append(const T& a, const T& b) {
		return a * b;
	}

	static T pow(const T& arg, uint power) {
		T ans = zero;
		T current = arg;
		while (power > 0) {
			if (power % 2)
				ans *= current;
			power /= 2;
			current = current * current;
		}
		return ans;
	}
};

template<typename T>
const T monoid_mul<T>::zero = T(1);

template<typename T>
struct monoid_min {
	typedef T type;
	static const T zero;

	static T append(const T& a, const T& b) {
		return std::min(a, b);
	}

	static T pow(const T& arg, uint power) {
		return (power == 0) ? zero : arg;
	}
};

template<typename T>
const T monoid_min<T>::zero = std::numeric_limits<T>::max();

template<typename T>
struct monoid_max {
	typedef T type;
	static const T zero;

	static T append(const T& a, const T& b) {
		return std::max(a, b);
	}

	static T pow(const T& arg, uint power) {
		return (power == 0) ? zero : arg;
	}
};

template<typename T>
const T monoid_max<T>::zero = std::numeric_limits<T>::min();