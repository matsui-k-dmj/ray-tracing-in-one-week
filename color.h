#pragma once

#include "vec3.h"
#include <iostream>

inline auto write_color(std::ostream& out, Color pixel_color) {
	auto i_r{ static_cast<int> (255.999 * pixel_color.x()) };
	auto i_g{ static_cast<int> (255.999 * pixel_color.y()) };
	auto i_b{ static_cast<int> (255.999 * pixel_color.z()) };

	out << i_r << ' ' << i_g << ' ' << i_b << '\n';
}