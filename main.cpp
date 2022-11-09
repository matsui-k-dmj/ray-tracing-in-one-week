#include <iostream>
int main() {
	constexpr auto image_width{ 256 };
	constexpr auto image_height{ 256 };

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (auto j{ image_height - 1 }; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << '\n' << std::flush;
		for (auto i{ 0 }; i < image_width; ++i) {
			auto d_r = static_cast<double>(i) / (static_cast<double>(image_width) - 1);
			auto d_g = static_cast<double>(j) / (static_cast<double>(image_height) - 1);
			auto d_b = 0.25;

			auto i_r = static_cast<int> (255.999 * d_r);
			auto i_g = static_cast<int> (255.999 * d_g);
			auto i_b = static_cast<int> (255.999 * d_b);

			std::cout << i_r << ' ' << i_g << ' ' << i_b << '\n';

		}
	}

	std::cerr << "\nDone.\n";
}