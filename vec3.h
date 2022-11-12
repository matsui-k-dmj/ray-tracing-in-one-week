#pragma once

#include<cmath>
#include<iostream>
#include <array>


class Vec3 {
private:
	std::array<double, 3> m_e{ 0, 0, 0 };

public:

	Vec3() : m_e{ 0,0,0 } {};

	Vec3(double e0, double e1, double e2) : m_e{ e0, e1, e2 } {};

	auto x() const ->  double { return m_e[0]; };
	auto y() const ->  double { return m_e[1]; };
	auto z() const ->  double { return m_e[2]; };

	auto operator-() const -> Vec3 { return { -m_e[0], -m_e[1], -m_e[2] }; };

	auto operator[](int i) const -> double { return m_e.at(i); };
	auto operator[](int i) -> double& { return m_e.at(i); };

	auto operator+= (const Vec3& v) -> Vec3& {
		m_e[0] += v.m_e[0];
		m_e[1] += v.m_e[1];
		m_e[2] += v.m_e[2];
		return *this;
	}

	auto operator*= (double s) -> Vec3& {
		m_e[0] *= s;
		m_e[1] *= s;
		m_e[2] *= s;
		return *this;
	}

	auto operator/= (double d) -> Vec3& {
		*this *= (1 / d);
		return *this;
	}

	auto length_squared() const -> double {
		return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2];
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