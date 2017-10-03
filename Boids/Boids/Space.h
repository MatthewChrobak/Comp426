#pragma once
#include "Vector2f.h"

#define WINDOW_WIDTH 1700
#define WINDOW_HEIGHT 960

class Space
{
public:
	static Vector2f ConvertForOpenGL(Vector2f v);
	static float ConvertWidth(float width);
	static float ConvertHeight(float height);
};

