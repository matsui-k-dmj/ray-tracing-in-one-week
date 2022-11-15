#pragma once

#include "vec3.h"
#include <iostream>
#include <algorithm>

inline auto write_color(std::ostream& out, Color pixel_color) {
	auto i_r{ static_cast<int> (255.999 * std::clamp(pixel_color.x(), 0.0, 0.999)) };
	auto i_g{ static_cast<int> (255.999 * std::clamp(pixel_color.y(), 0.0, 0.999)) };
	auto i_b{ static_cast<int> (255.999 * std::clamp(pixel_color.z(), 0.0, 0.999)) };

	out << i_r << ' ' << i_g << ' ' << i_b << '\n';
}