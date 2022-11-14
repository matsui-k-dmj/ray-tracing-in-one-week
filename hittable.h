#pragma once

#include "ray.h"

struct HitRecord {
	Point3 point{};
	Vec3 normal{};
	double t{};
	bool is_front_face{};
};

inline auto set_face_normal(const Ray& ray, const Vec3& outword_normal, HitRecord& record) {
	// ray と 外向きの法線ベクトルが 反対を向いていれば rayは表から当たってる
	record.is_front_face = dot(ray.m_direction, outword_normal) < 0;
	// 法線ベクトルが ray がどっちの面から当たっていても ray の反対向きになるようにする。
	record.normal = record.is_front_face ? outword_normal : -outword_normal;

}

class Hittable {
public:
	virtual auto hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const
		-> bool = 0;
};