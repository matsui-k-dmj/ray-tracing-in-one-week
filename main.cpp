#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include "utils.h"

#include <memory>
#include <iostream>

auto do_hit_sphere(const Point3& sphere_center, double sphere_radius, const Ray& ray)-> bool {
	Vec3 center_to_origin = ray.m_origin - sphere_center;
	auto a = dot(ray.m_direction, ray.m_direction);
	auto b = 2.0 * dot(center_to_origin, ray.m_direction);
	auto c = dot(center_to_origin, center_to_origin) - sphere_radius * sphere_radius;
	auto discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

auto hit_sphere(const Point3& sphere_center, double sphere_radius, const Ray& ray)-> double {
	Vec3 center_to_origin = ray.m_origin - sphere_center;
	auto a = dot(ray.m_direction, ray.m_direction);
	auto b = 2.0 * dot(center_to_origin, ray.m_direction);
	auto c = dot(center_to_origin, center_to_origin) - sphere_radius * sphere_radius;
	auto discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		// smaller t
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}

auto get_ray_color(const Ray& ray, const Hittable& world)-> Color {


	HitRecord hit_record{};
	auto do_hit = world.hit(ray, 0.0, constants::infinity, hit_record);


	if (do_hit) {
		// sphere の法線ベクトルの可視化
		auto normal_unit_vec = hit_record.normal;
		return 0.5 * Color{
			normal_unit_vec.x() + 1.0, normal_unit_vec.y() + 1.0, normal_unit_vec.z() + 1.0, };
	}

	// 背景
	// 原文では unit vectorを取ってるけどそれはイミフ
	// y が [-1, 1] なのを t [0, 1]に変換
	double a = 0.5 * (ray.m_direction.y() + 1.0);
	return (1.0 - a) * Color { 1.0, 1.0, 1.0 } + a * Color(0.5, 0.7, 1.0);
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
		camera_origin - Vec3{0, 0, focal_length} - viewport_horizontal_vec / 2 - viewport_vertical_vec / 2 };

	HittableList world{};
	world.add(std::make_shared<Sphere>(Point3{ 0, 0, -1 }, 0.5));
	world.add(std::make_shared<Sphere>(Point3{ 0, 0.2, -1 }, 0.5));

	world.add(std::make_shared<Sphere>(Point3{ 0, -100.5, -1 }, 100.0)); // 地面


	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (auto j{ image_height - 1 }; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << '\n' << std::flush;
		for (auto i{ 0 }; i < image_width; ++i) {
			auto w{ static_cast<double>(i) / (static_cast<double>(image_width) - 1) };
			auto h{ static_cast<double>(j) / (static_cast<double>(image_height) - 1) };

			Ray ray{ camera_origin,
				(viewport_lower_left_corner
				+ w * viewport_horizontal_vec
				+ h * viewport_vertical_vec
				- camera_origin).unit() };

			write_color(std::cout, get_ray_color(ray, world));
		}
	}

	std::cerr << "\nDone.\n";
}