#include "Vector2f.h"
#include <math.h>

Vector::Vector() : Vector(0, 0)
{
}

Vector::Vector(float x, float y)
{
	this->X = x;
	this->Y = y;
}

Vector Vector::operator+(Vector v2)
{
	Vector result(v2.X, v2.Y);
	result.X += this->X;
	result.Y += this->Y;
	return result;
}

Vector Vector::operator-(Vector v2)
{
	Vector result(this->X, this->Y);
	result.X -= v2.X;
	result.Y -= v2.Y;
	return result;
}

Vector Vector::operator/(float val)
{
	Vector result(this->X, this->Y);
	result.X /= val;
	result.Y /= val;
	return result;
}

Vector Vector::operator*(float val)
{
	Vector result(this->X, this->Y);
	result.X *= val;
	result.Y *= val;
	return result;
}

Vector Vector::operator+=(Vector v2)
{
	*this = *this + v2;
	return *this;
}

Vector Vector::operator-=(Vector v2)
{
	*this = *this - v2;
	return *this;
}

Vector Vector::operator/=(float val)
{
	*this = *this / val;
	return *this;
}

Vector Vector::operator*=(float val)
{
	*this = *this * val;
	return *this;
}

float Vector::norm()
{
	return (float)sqrt(this->X * this->X + this->Y * this->Y);
}
