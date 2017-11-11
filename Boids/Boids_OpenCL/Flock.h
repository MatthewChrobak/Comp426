#pragma once
#include "Boid.h"
#include "Color.h"
#include <vector>

struct Flock
{
	std::vector<Boid*> Boids;
	Color Color;

	void updateBoids();
};