#include "Vector2f.h"
#include <math.h>

Vector2f::Vector2f() : Vector2f(0, 0)
{
}

Vector2f::Vector2f(float x, float y)
{
	this->X = x;
	this->Y = y;
}

Vector2f Vector2f::operator+(Vector2f v2)
{
	Vector2f result(v2.X, v2.Y);
	result.X += this->X;
	result.Y += this->Y;
	return result;
}

Vector2f Vector2f::operator-(Vector2f v2)
{
	Vector2f result(this->X, this->Y);
	result.X -= v2.X;
	result.Y -= v2.Y;
	return result;
}

Vector2f Vector2f::operator/(float val)
{
	Vector2f result(this->X, this->Y);
	result.X /= val;
	result.Y /= val;
	return result;
}

Vector2f Vector2f::operator*(float val)
{
	Vector2f result(this->X, this->Y);
	result.X *= val;
	result.Y *= val;
	return result;
}

//Vector2f Vector2f::operator+=(Vector2f v2)
//{
//	*this = *this + v2;
//	return *this;
//}
//
//Vector2f Vector2f::operator-=(Vector2f v2)
//{
//	*this = *this - v2;
//	return *this;
//}
//
//Vector2f Vector2f::operator/=(float val)
//{
//	*this = *this / val;
//	return *this;
//}
//
//Vector2f Vector2f::operator*=(float val)
//{
//	*this = *this * val;
//	return *this;
//}

float Vector2f::norm()
{
	return sqrt(this->X * this->X + this->Y * this->Y);
}
