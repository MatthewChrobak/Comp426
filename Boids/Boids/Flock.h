#pragma once
#include "Vector2f.h"
#include <vector>
#include "Boid.h"

class Flock
{
public:
	char R;
	char G;
	char B;

	std::vector<Boid*> Boids;
	bool RequiresLogicUpdate = false;
};

