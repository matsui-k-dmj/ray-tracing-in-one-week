#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

auto get_ray_color(const Ray& ray)-> Color {
	// 原文では unit vectorを取ってるけどそれはイミフ
	// y が [-1, 1] なのを t [0, 1]に変換
	double t = 0.5 * (ray.m_direction.y() + 1.0);
	return (1.0 - t) * Color { 1.0, 1.0, 1.0 } + t * Color(0.5, 0.7, 1.0);
}

int main() {
	constexpr double width_over_height_ratio{ 16.0 / 9.0 };
	constexpr int image_width{ 400 };
	constexpr int image_height{ static_cast<int>(image_width / width_over_height_ratio) };

	constexpr double viewport_height{ 2.0 };
	constexpr double viewport_width{ viewport_height * width_over_height_ratio };
	constexpr double focal_length{ 1.0 };

	const Vec3 viewport_horizontal_vec{ viewport_width, 0, 0 };
	const Vec3 viewport_vertical_vec{ 0, viewport_height, 0 };

	const Point3 camera_origin{ 0, 0, 0 };
	const Point3 viewport_lower_left_corner{
		camera_origin - Vec3{0, 0, -focal_length} - viewport_horizontal_vec / 2 - viewport_vertical_vec / 2 };


	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (auto j{ image_height - 1 }; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << '\n' << std::flush;
		for (auto i{ 0 }; i < image_width; ++i) {
			auto w{ static_cast<double>(i) / (static_cast<double>(image_width) - 1) };
			auto h{ static_cast<double>(j) / (static_cast<double>(image_height) - 1) };

			Ray ray{ camera_origin,
				viewport_lower_left_corner
				+ w * viewport_horizontal_vec
				+ h * viewport_vertical_vec
				- camera_origin };

			write_color(std::cout, get_ray_color(ray));
		}
	}

	std::cerr << "\nDone.\n";
}