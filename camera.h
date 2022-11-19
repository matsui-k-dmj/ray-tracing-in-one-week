#pragma once

#include "vec3.h"
#include "ray.h"

class Camera {
private:
	Point3 m_camera_origin{};
	Vec3 m_viewport_horizontal_vec{};
	Vec3 m_viewport_vertical_vec{};
	Point3 m_viewport_lower_left_corner{};

public:
	Camera(
		Point3 camera_origin,
		Point3 look_at,
		Vec3 up_vec = { 0, 1, 0 },
		double width_over_height_ratio = 16.0 / 9.0,
		double fov_degree = 45) : m_camera_origin{ camera_origin }
	{
		const double viewport_height{ 2.0 * std::tan(degrees_to_radians(fov_degree) / 2) };
		const double viewport_width{ viewport_height * width_over_height_ratio };

		const Vec3 camera_direction = (look_at - camera_origin).unit();
		const Vec3 horizontal_camera_vec = cross(camera_direction, up_vec).unit();
		const Vec3 vertical_camera_vec = cross(horizontal_camera_vec, camera_direction).unit();

		auto viewport_origin = camera_origin + camera_direction;


		m_viewport_horizontal_vec = viewport_width * horizontal_camera_vec;
		m_viewport_vertical_vec = viewport_height * vertical_camera_vec;
		m_viewport_lower_left_corner =
			viewport_origin - m_viewport_horizontal_vec / 2 - m_viewport_vertical_vec / 2;


	}

	auto get_ray(double horizontal_ratio, double vertical_ratio) const -> Ray {
		return { m_camera_origin,
			m_viewport_lower_left_corner
			+ horizontal_ratio * m_viewport_horizontal_vec
			+ vertical_ratio * m_viewport_vertical_vec
			- m_camera_origin };
	}
};