#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "camera.h"

#include "utils.h"

#include <memory>
#include <iostream>

auto visualize_normal_vec(const Vec3& normal_vec) -> Color {
	auto normal_unit_vec = normal_vec.unit();
	return 0.5 * Color{
				normal_unit_vec.x() + 1.0, normal_unit_vec.y() + 1.0, normal_unit_vec.z() + 1.0, };
}

auto get_ray_color(const Ray& ray, const Hittable& world, int n_reflection_available)-> Color {
	if (n_reflection_available <= 0)
		return{ 0, 0, 0 };

	HitRecord hit_record{};
	constexpr auto t_very_close = 0.001;
	auto do_hit = world.hit(ray, 0.0, constants::infinity, hit_record);
	if (do_hit) {
		if (hit_record.t > t_very_close) {
			auto reflectDirection = random_vec_in_unit_sphere().unit() + hit_record.normal;

			auto reflect_ratio = 0.3;
			return reflect_ratio * get_ray_color(Ray{ hit_record.point, reflectDirection }, world, n_reflection_available - 1);
		}
		else {
			return { 0, 0, 0 };
		}
	}

	// 背景
	// y が [-1, 1] なのを t [0, 1]に変換
	double a = 0.5 * (ray.m_direction.y() + 1.0);
	return (1.0 - a) * Color { 1.0, 1.0, 1.0 } + a * Color(0.5, 0.7, 1.0);
}

int main() {

	constexpr double width_over_height_ratio{ 16.0 / 9.0 };
	constexpr int image_width{ 400 };
	constexpr int image_height{ static_cast<int>(image_width / width_over_height_ratio) };

	constexpr int n_samples_per_pixel = 100;
	constexpr int max_refletion = 30;

	HittableList world{};
	world.add(std::make_shared<Sphere>(Point3{ 0, 0, -1 }, 0.5));
	world.add(std::make_shared<Sphere>(Point3{ 0, -100.5, -1 }, 100.0)); // 地面

	Camera camera{};

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (auto j{ image_height - 1 }; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << '\n' << std::flush;
		for (auto i{ 0 }; i < image_width; ++i) {
			Color color{ 0, 0, 0 };
			for (auto i_sample{ 0 }; i_sample < n_samples_per_pixel; ++i_sample) {
				auto w{ (static_cast<double>(i) + random_1()) / (static_cast<double>(image_width) - 1) };
				auto h{ (static_cast<double>(j) + random_1()) / (static_cast<double>(image_height) - 1) };
				auto ray = camera.get_ray(w, h);
				color += get_ray_color(ray, world, max_refletion);
			}

			write_color(std::cout, color / n_samples_per_pixel);
		}
	}

	std::cerr << "\nDone.\n";
}