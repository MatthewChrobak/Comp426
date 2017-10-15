#include "RNG.h"
#include <random>
#include <time.h>


int RNG::getNextInt(int lb, int ub)
{
	static std::mt19937 rng(time(0));
	return (rng() % (ub - lb)) + lb;
}