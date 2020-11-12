#include "Util.h"

namespace utl
{
	inline double clamp(double val, double min = 0.0, double max = 1.0)
	{
		if (val >= max)
			return max;
		if (val <= min)
			return min;
		return val;
	}

	inline double loop(double val, double min = 0.0, double max = 1.0)
	{
		if (val >= max)
			return val + min - max;
		if (val < min)
			return  max - min + val;
		return val;
	}

	inline double random(double min = 0.0, double max = 1.0)
	{
		return (double)rand() / RAND_MAX * (max - min) + min;
	}
}