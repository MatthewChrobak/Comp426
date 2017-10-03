#pragma once
class Vector2f
{
public:
	float X;
	float Y;

	Vector2f();
	Vector2f(float x, float y);

	Vector2f operator+(Vector2f v2);
	Vector2f operator-(Vector2f v2);
	Vector2f operator/(float val);
	Vector2f operator*(float val);
	//Vector2f operator+=(Vector2f v2);
	//Vector2f operator-=(Vector2f v2);
	//Vector2f operator/=(float val);
	//Vector2f operator*=(float val);

	float norm();
};

