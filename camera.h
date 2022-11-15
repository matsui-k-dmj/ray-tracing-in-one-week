#pragma once

#include "vec3.h"
#include "ray.h"

class Camera {
private:
	Vec3 m_viewport_horizontal_vec{};
	Vec3 m_viewport_vertical_vec{};
	Point3 m_camera_origin{};
	Point3 m_viewport_lower_left_corner{};

public:
	Camera() {
		constexpr double width_over_height_ratio{ 16.0 / 9.0 };
		constexpr double viewport_height{ 2.0 };
		constexpr double viewport_width{ viewport_height * width_over_height_ratio };
		constexpr double focal_length{ 1.0 };

		m_viewport_horizontal_vec = { viewport_width, 0, 0 };
		m_viewport_vertical_vec = { 0, viewport_height, 0 };
		m_camera_origin = { 0, 0, 0 };
		m_viewport_lower_left_corner =
			m_camera_origin - Vec3{ 0, 0, focal_length }
		- m_viewport_horizontal_vec / 2 - m_viewport_vertical_vec / 2;

	}

	auto get_ray(double horizontal_ratio, double vertical_ratio) const -> Ray {
		return { m_camera_origin,
			m_viewport_lower_left_corner
			+ horizontal_ratio * m_viewport_horizontal_vec
			+ vertical_ratio * m_viewport_vertical_vec
			- m_camera_origin };
	}
};