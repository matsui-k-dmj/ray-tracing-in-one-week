#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "camera.h"

#include "utils.h"

#include <memory>
#include <iostream>

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}

auto visualize_normal_vec(const Vec3& normal_vec) -> Color {
	auto normal_unit_vec = normal_vec.unit();
	return 0.5 * Color{
				normal_unit_vec.x() + 1.0, normal_unit_vec.y() + 1.0, normal_unit_vec.z() + 1.0, };
}

auto get_ray_color(const Ray& ray, const Hittable& world, int n_reflection_available)-> Color {
	if (n_reflection_available <= 0)
		return{ 0, 0, 0 };

	HitRecord hit_record{};
	constexpr auto t_very_close = 1e-8;

	auto do_hit = world.hit(ray, 0.0, constants::infinity, hit_record);
	if (do_hit) {
		if (hit_record.t > t_very_close) {

			Color attenuation_color{};
			Ray ray_scattered{};
			auto is_scattered = hit_record.material_ptr->scatter(ray, hit_record, attenuation_color, ray_scattered);

			if (is_scattered) {

				return attenuation_color * get_ray_color(ray_scattered, world, n_reflection_available - 1);
			}
		} {
			return { 0, 0, 0 };

		}
	}

	// 背景
	// y が [-1, 1] なのを t [0, 1]に変換
	double a = 0.5 * (ray.m_direction.unit().y() + 1.0);
	return (1.0 - a) * Color { 1.0, 1.0, 1.0 } + a * Color(0.5, 0.7, 1.0);
}

int main() {

	constexpr double width_over_height_ratio{ 16.0 / 9.0 };
	constexpr int image_width{ 400 };
	constexpr int image_height{ static_cast<int>(image_width / width_over_height_ratio) };

	constexpr int n_samples_per_pixel = 100;
	constexpr int max_refletion = 10;

	auto ground_material = std::make_shared<Lambertian>(Color{ 0, 0.1, 0.1 });
	auto matt_material = std::make_shared<Lambertian>(Color{ 0.98, 0.51, 0.48 });
	auto metal_material = std::make_shared<Metal>(Color{ 0.8, 0.8, 0.8 });
	auto metal_perfect_material = std::make_shared<Metal>(Color{ 1.0, 1.0, 1.0 });


	HittableList world{};
	world.add(std::make_shared<Sphere>(Point3{ 0, 0, -1.5 }, 0.5, matt_material));
	world.add(std::make_shared<Sphere>(Point3{ 1, 0, -1.0 }, 0.5, metal_material));
	world.add(std::make_shared<Sphere>(Point3{ -0.7, 0, -0.5 }, 0.5, metal_perfect_material));

	world.add(std::make_shared<Sphere>(Point3{ 0, -100.5, -1 }, 100.0, ground_material)); // 地面

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