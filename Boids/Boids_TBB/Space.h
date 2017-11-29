#pragma once
#include "Vector2f.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640

class Space
{
public:
	static Vector2f ConvertForOpenGL(Vector2f v);
	static float ConvertWidth(float width);
	static float ConvertHeight(float height);
};

