#pragma once
#include "Vector2f.h"
class Boid
{
public:
	Vector CurPos;
	Vector Velocity;
	bool RequiresLogicUpdate = false;
	bool HasUpdatedLogic = false;
};

