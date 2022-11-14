#pragma once

#include <limits>
#include <cstdlib>

namespace constants
{
	inline constexpr double pi{ 3.1415926535897932385 };
	inline constexpr double infinity = std::numeric_limits<double>::infinity();
}

inline auto degrees_to_radians(double degrees) -> double {
	return degrees * constants::pi / 180.0;
}

// random double from [0, 1)
inline auto random_1() -> double {
	return rand() / (RAND_MAX + 1.0);
}

// random double from [min, max)
inline auto random_from_range(double min, double max) -> double {
	return min + (max - min) * random_1();
}