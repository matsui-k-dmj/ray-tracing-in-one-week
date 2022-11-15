#pragma once
#include "utils.h"

#include<cmath>
#include<iostream>
#include <array>


class Vec3 {
private:
	std::array<double, 3> e{ 0, 0, 0 };

public:

	Vec3() : e{ 0,0,0 } {};

	Vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {};

	auto x() const ->  double { return e[0]; };
	auto y() const ->  double { return e[1]; };
	auto z() const ->  double { return e[2]; };

	auto operator-() const -> Vec3 { return { -e[0], -e[1], -e[2] }; };

	auto operator[](int i) const -> double { return e.at(i); };
	auto operator[](int i) -> double& { return e.at(i); };

	auto operator+= (const Vec3& v) -> Vec3& {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	auto operator*= (double s) -> Vec3& {
		e[0] *= s;
		e[1] *= s;
		e[2] *= s;
		return *this;
	}

	auto operator/= (double d) -> Vec3& {
		*this *= (1 / d);
		return *this;
	}

	auto length_squared() const -> double {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	auto length() const -> double {
		return std::sqrt(length_squared());
	}

	auto unit() const->Vec3;

};

inline auto operator<<(std::ostream& out, const Vec3& v) -> std::ostream& {
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline auto operator+(const Vec3& a, const Vec3& b) -> Vec3 {
	return { a[0] + b[0], a[1] + b[1], a[2] + b[2] };
}

inline auto operator-(const Vec3& a, const Vec3& b) -> Vec3 {
	return { a[0] - b[0], a[1] - b[1], a[2] - b[2] };
}

inline auto operator*(const Vec3& a, const Vec3& b) -> Vec3 {
	return { a[0] * b[0], a[1] * b[1], a[2] * b[2] };
}

inline auto operator*(double s, const Vec3& v) -> Vec3 {
	return { s * v[0], s * v[1], s * v[2] };
};

inline auto operator*(const Vec3& v, double s) -> Vec3 {
	return s * v;
}

inline auto operator/(const Vec3& v, double d) -> Vec3 {
	return v * (1 / d);
}

inline auto dot(const Vec3& a, const Vec3& b) -> double {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline auto cross(const Vec3& a, const Vec3& b) -> Vec3 {
	return {
		a.y() * b.z() - a.z() * b.y(),
		a.z() * b.x() - a.x() * b.z(),
		a.x() * b.y() - a.y() * b.x(),
	};
}

// operator/ を定義してからじゃないと定義できないから、クラスのそとでやる
inline auto Vec3::unit() const->Vec3 {
	return (*this) / length();
}


using Point3 = Vec3;

using Color = Vec3;

inline auto random_vec_from_range(double min, double max) -> Vec3 {
	return { random_from_range(min, max), random_from_range(min, max), random_from_range(min, max) };
}

inline auto random_vec_in_unit_sphere() -> Vec3 {
	while (true) {
		auto vec = random_vec_from_range(-1.0, 1.0);
		if (vec.length_squared() < 1.0) {
			return vec;
		}
		return vec;
	}
}