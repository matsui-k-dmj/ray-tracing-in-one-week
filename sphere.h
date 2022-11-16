#pragma once

#include "hittable.h"
#include "vec3.h"
#include "material.h"

#include <memory>

class Sphere : public Hittable {
public:
	Point3 m_center{};
	double m_radius{};
	std::shared_ptr<Material> m_material_ptr{};

	Sphere() = default;
	Sphere(const Point3& center, double radius, std::shared_ptr<Material> material_ptr)
		: m_center{ center }, m_radius{ radius }, m_material_ptr{ material_ptr }
	{};

	virtual auto hit(const Ray& ray, double t_min, double t_max, HitRecord& out_record) const
		-> bool override;

};

inline auto Sphere::hit(const Ray& ray, double t_min, double t_max, HitRecord& out_record) const
-> bool
{
	Vec3 center_to_origin = ray.m_origin - m_center;
	auto a = dot(ray.m_direction, ray.m_direction);
	auto b = 2.0 * dot(center_to_origin, ray.m_direction);
	auto c = dot(center_to_origin, center_to_origin) - m_radius * m_radius;
	auto discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return false;
	}

	auto smaller_t = (-b - sqrt(discriminant)) / (2.0 * a);
	if (smaller_t < t_min || t_max < smaller_t) {
		return false;
	}

	out_record.t = smaller_t;
	out_record.point = ray.at(out_record.t);
	Vec3 outword_nomal = (out_record.point - m_center) / m_radius;
	set_face_normal(ray, outword_nomal, out_record);
	out_record.material_ptr = m_material_ptr;

	return true;
}