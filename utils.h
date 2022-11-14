#pragma once

#include <limits>

namespace constants
{
	inline constexpr double pi{ 3.1415926535897932385 };
	inline constexpr double infinity = std::numeric_limits<double>::infinity();
}

inline auto degrees_to_radians(double degrees) -> double {
	return degrees * constants::pi / 180.0;
}