#pragma once
#include "DataTypes.h"

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 900

class Space
{
public:
	static Vector ConvertForOpenGL(Vector v);
	static float ConvertWidth(float width);
	static float ConvertHeight(float height);
};

