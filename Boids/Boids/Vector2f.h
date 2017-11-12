#pragma once
class Vector
{
public:
	float X;
	float Y;

	Vector();
	Vector(float x, float y);

	Vector operator+(Vector v2);
	Vector operator-(Vector v2);
	Vector operator/(float val);
	Vector operator*(float val);
	Vector operator+=(Vector v2);
	Vector operator-=(Vector v2);
	Vector operator/=(float val);
	Vector operator*=(float val);

	float norm();
};

