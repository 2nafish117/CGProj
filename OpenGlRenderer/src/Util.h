#pragma once

#include <cstdlib>

namespace utl
{
	extern inline double clamp(double val, double min, double max);

	extern inline double loop(double val, double min, double max);

	extern inline double random(double min, double max);
}