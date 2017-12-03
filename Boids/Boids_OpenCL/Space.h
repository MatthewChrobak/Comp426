#pragma once
#include "DataTypes.h"
#include "Constants.h"

class Space
{
public:
	static Vector ConvertForOpenGL(Vector v);
	static float ConvertWidth(float width);
	static float ConvertHeight(float height);
};

