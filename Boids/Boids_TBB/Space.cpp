#include "Space.h"

Vector Space::ConvertForOpenGL(Vector v)
{
	return Vector(Space::ConvertWidth(v.X), Space::ConvertHeight(v.Y));
}

float Space::ConvertHeight(float y)
{
	int midpointY = WINDOW_HEIGHT / 2;

	// Figure out what quadrant it's in.
	if (y < midpointY) {
		return 1 - y / midpointY;
	}
	else {
		return -(y - midpointY) / midpointY;
	}
}

float Space::ConvertWidth(float x)
{
	int midpointX = WINDOW_WIDTH / 2;

	// Figure out what quadrant it's in.
	if (x < midpointX) {
		return -1 + x / midpointX;
	}
	else {
		return (x - midpointX) / midpointX;
	}
}