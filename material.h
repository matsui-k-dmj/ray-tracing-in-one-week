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
	double m_fuzz{};

	Metal(const Color& attenuation, double fuzz = 0)
		: m_attenuation{ attenuation }, m_fuzz{ fuzz }
	{};

	virtual auto scatter(const Ray& ray_in, const HitRecord& hit_record, Color& out_attenuation, Ray& out_ray_scattered) const
		-> bool override {
		auto reflectDirection = reflect(ray_in.m_direction.unit(), hit_record.normal);
		out_ray_scattered = Ray{ hit_record.point, reflectDirection + random_vec_in_unit_sphere() * m_fuzz };
		out_attenuation = m_attenuation;
		return dot(reflectDirection.unit(), hit_record.normal.unit()) > 0;
	}
};

class Glass : public Material {
public:
	double m_index_of_refraction{};
	Glass(double index_of_refraction)
		: m_index_of_refraction{ index_of_refraction }
	{};

	virtual auto scatter(
		const Ray& ray_in, const HitRecord& hit_record, Color& out_attenuation, Ray& out_ray_scattered) const
		-> bool override {
		out_attenuation = { 1.0, 1.0, 1.0 };
		double etain_over_etaout = hit_record.is_front_face ? (1.0 / m_index_of_refraction) : m_index_of_refraction;

		auto in_unit_vec = ray_in.m_direction.unit();
		auto cos_theta = dot(-in_unit_vec, hit_record.normal);
		double sin_theta = std::sqrt(1 - cos_theta * cos_theta);

		bool cannot_refract = etain_over_etaout * sin_theta > 1.0;


		Vec3 out_direction{};
		if (cannot_refract || reflectance(cos_theta, etain_over_etaout) > random_1()) {
			out_direction = reflect(ray_in.m_direction.unit(), hit_record.normal);
		}
		else {
			out_direction = refract(in_unit_vec, hit_record.normal, etain_over_etaout);
		}

		out_ray_scattered = Ray{ hit_record.point, out_direction };

		return true;
	}
private:
	double reflectance(double cosine, double ref_idx) const {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

};