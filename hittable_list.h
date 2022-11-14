#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
	std::vector<std::shared_ptr<Hittable>> m_object_list{};

	HittableList() = default;

	auto clear() -> void
	{
		m_object_list.clear();
	}

	auto add(std::shared_ptr<Hittable> object) -> void
	{
		m_object_list.push_back(object);
	}

	virtual auto hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const
		-> bool override;
};

inline auto HittableList::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const
-> bool
{
	HitRecord temp_hit_record{};
	auto do_hit_anything = false;
	auto t_closest = t_max;

	for (const auto& object : m_object_list) {
		if (object->hit(ray, t_min, t_closest, temp_hit_record)) {
			do_hit_anything = true;
			t_closest = temp_hit_record.t;
			record = temp_hit_record;
		}
	}
	return do_hit_anything;
}