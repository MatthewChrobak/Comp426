#pragma once
#include "Vector2f.h"
class Boid
{
public:
	Boid();

	Vector2f CurPos;
	Vector2f NewPos;
	Vector2f Velocity;
};

