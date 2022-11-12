#include "color.h"
#include "vec3.h"

#include <iostream>

int main() {
	constexpr auto image_width{ 256 };
	constexpr auto image_height{ 256 };


	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (auto j{ image_height - 1 }; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << '\n' << std::flush;
		for (auto i{ 0 }; i < image_width; ++i) {
			auto d_r{ static_cast<double>(i) / (static_cast<double>(image_width) - 1) };
			auto d_g{ static_cast<double>(j) / (static_cast<double>(image_height) - 1) };
			auto d_b{ 0.25 };

			write_color(std::cout, { d_r, d_g, d_b });
		}
	}

	std::cerr << "\nDone.\n";
}