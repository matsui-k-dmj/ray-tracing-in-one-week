#pragma once

#include "vec3.h"

class Ray {
public:
	Point3 m_origin{};
	Vec3 m_direction{};

	Ray(const Point3& origin, const Vec3& direction)
		: m_origin{ origin }, m_direction{ direction.unit() } {}

	auto at(double t) const -> Point3 {
		return m_origin + t * m_direction;
	}
};