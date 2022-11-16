#pragma once

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

class Material {
public:
	virtual auto scatter(const Ray& ray_in, const HitRecord& hit_record, Color& out_attenuation, Ray& out_ray_scattered) const
		-> bool = 0;
};

class Lambertian : public Material {
public:
	Color m_attenuation{};

	Lambertian(const Color& attenuation) : m_attenuation{ attenuation } {};

	virtual auto scatter(const Ray& /*ray_in*/, const HitRecord& hit_record, Color& out_attenuation, Ray& out_ray_scattered) const
		-> bool override {
		auto reflectDirection = random_vec_in_unit_sphere().unit() + hit_record.normal;
		out_ray_scattered = Ray{ hit_record.point, reflectDirection };
		out_attenuation = m_attenuation;
		return true;
	}
};

class Metal : public Material {
public:
	Color m_attenuation{};

	Metal(const Color& attenuation) : m_attenuation{ attenuation } {};

	virtual auto scatter(const Ray& ray_in, const HitRecord& hit_record, Color& out_attenuation, Ray& out_ray_scattered) const
		-> bool override {
		auto reflectDirection = reflect(ray_in.m_direction.unit(), hit_record.normal);
		out_ray_scattered = Ray{ hit_record.point, reflectDirection };
		out_attenuation = m_attenuation;
		return dot(reflectDirection.unit(), hit_record.normal.unit()) > 0;
	}
};