#pragma once
#include "Vector2f.h"
class Boid
{
public:
	Vector2f CurPos;
	Vector2f NewPos;
	Vector2f Velocity;
	bool RequiresLogicUpdate = false;
	bool HasUpdatedLogic = false;
};

