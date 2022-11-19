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
	constexpr auto t_very_close = 1e-10;

	auto do_hit = world.hit(ray, t_very_close, constants::infinity, hit_record);
	if (do_hit) {
		if (hit_record.material_ptr->m_is_emitting) {
			return hit_record.material_ptr->m_emittingColor;
		}
		else {
			Color attenuation_color{};
			Ray ray_scattered{};
			auto is_scattered = hit_record.material_ptr->scatter(ray, hit_record, attenuation_color, ray_scattered);

			if (is_scattered) {
				return attenuation_color * get_ray_color(ray_scattered, world, n_reflection_available - 1);
			}
			else {
				return { 0, 0, 0 };
			}
		}
	}

	// 背景
	// y が [-1, 1] なのを t [0, 1]に変換
	double a = (std::clamp(ray.m_direction.unit().y(), -0.1, 0.15) + 0.1) / 0.25;
	return ((1.0 - a) * Color { 168, 62, 117 } *0.5 / 256 + a * Color(0, 16, 107) / 256);
}

int main() {
	constexpr bool DEBUG = false;

	constexpr double width_over_height_ratio{ 16.0 / 9.0 };
	constexpr int image_width{ DEBUG ? 400 : 1280 };
	constexpr int image_height{ static_cast<int>(image_width / width_over_height_ratio) };

	constexpr int n_samples_per_pixel = DEBUG ? 100 : 1000;
	constexpr int max_refletion = DEBUG ? 10 : 100;

	auto ground_material = std::make_shared<Lambertian>(Color{ 0.5, 0.5, 0.5 });
	auto light_material = std::make_shared<Light>(Color{ 1, 1, 1 });
	auto metal_material = std::make_shared<Metal>(Color{ 0.8, 0.8, 0.8 }, 0.3);
	auto metal_perfect_material = std::make_shared<Metal>(Color{ 1.0, 1.0, 1.0 });
	auto glass_material = std::make_shared<Glass>(1.5);

	HittableList world{};
	world.add(std::make_shared<Sphere>(Point3{ 0, 0.5, -1.5 }, 0.5, light_material));
	world.add(std::make_shared<Sphere>(Point3{ 1.4, 0.5, -2 }, 0.5, metal_material));

	world.add(std::make_shared<Sphere>(Point3{ 0.4, 0.5, -0.3 }, 0.5, glass_material));
	world.add(std::make_shared<Sphere>(Point3{ 0.4, 0.5, -0.3 }, -0.45, glass_material));
	world.add(std::make_shared<Sphere>(Point3{ 1.5, 0.5, -0.3 }, 0.5, glass_material));

	world.add(std::make_shared<Sphere>(Point3{ -0.7, 0.5, -0.5 }, 0.5, metal_perfect_material));

	world.add(std::make_shared<Sphere>(Point3{ 0, -1000, -1 }, 1000, ground_material)); // 地面

	for (double x = -7; x < 5; x += 0.9) {
		for (double z = -9; z < 5; z += 0.9) {
			auto radius = 0.18;
			auto rand_x = x + random_from_range(-0.3, 0.3);
			auto rand_z = z + random_from_range(-0.3, 0.3);
			if (-0.5 <= rand_x && rand_x <= 2 && -2 <= rand_z && rand_z <= 0.1) { continue; }

			auto center = Point3{ rand_x, radius, rand_z };

			// [0, 1)でサンプリングしてから彩度を上げてる
			auto color = random_vec_from_range(0., 1);
			auto max = *std::max_element(color.e.begin(), color.e.end());
			color /= max;
			color = { color.x() * color.x(), color.y() * color.y(), color.z() * color.z() };


			auto p = random_from_range(0, 1);
			if (p < 0.1) {
				world.add(std::make_shared<Sphere>(center, radius,
					std::make_shared<Lambertian>(color)));
			}
			else if (p < 0.3) {
				world.add(std::make_shared<Sphere>(center, radius,
					std::make_shared<Light>(color)));
			}
			else if (p < 0.5) {
				world.add(std::make_shared<Sphere>(center, radius,
					std::make_shared<Metal>(Color{ 1.0, 1.0, 1.0 })));
			}
			else if (p < 0.8) {
				world.add(std::make_shared<Sphere>(center, radius,
					std::make_shared<Metal>(color, random_from_range(0, 0.5))));
			}
			else if (p < 1) {
				world.add(std::make_shared<Sphere>(center, radius,
					std::make_shared<Glass>(1.5)));
			}
		}
	}

	Camera camera{ Point3{5, 1, 5}, Point3{-0.5, 0, -0.5}, {0, 1, 0}, width_over_height_ratio, 30 };


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