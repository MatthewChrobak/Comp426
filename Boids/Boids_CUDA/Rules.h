#pragma once
#include "DataTypes.h"
#include "Space.h"
#include <math.h>
#define BOOST_SPEED 5
#define BOID_RADIUS 7.0f

Vector2f add(Vector2f v1, Vector2f v2)
{
	v1.X += v2.X;
	v1.Y += v2.Y;
	return v1;
}

Vector2f subtract(Vector2f v1, Vector2f v2)
{
	v1.X -= v2.X;
	v1.Y -= v2.Y;
	return v1;
}

Vector2f multiply(Vector2f v1, float scalar)
{
	v1.X *= scalar;
	v1.Y *= scalar;
	return v1;
}

Vector2f divide(Vector2f v1, float scalar)
{
	v1.X /= scalar;
	v1.Y /= scalar;
	return v1;
}

float norm(Vector2f v)
{
	return sqrt(v.X * v.X + v.Y * v.Y);
}

void updatePosition(Boid* boid)
{
	boid->Position = add(boid->Position, boid->Velocity);
}

Vector2f cohesionRule(Boid* boids, int flockNum, int num_boids, Boid* thisBoid)
{
	Vector2f result;
	result.X = 0;
	result.Y = 0;

	for (int i = 0; i < num_boids; i++) {
		auto boid = (boids + (flockNum * num_boids) + i);

		if (boid != thisBoid) {
			result = add(result, boid->Position);
		}
	}

	if (norm(result) == 0) {
		return result;
	}

	result = divide(result, (float)(num_boids - 1));
	return divide(subtract(result, thisBoid->Position), 100.0f);
}

Vector2f separationRule(Boid* boids, int flockNum, int num_boids, Boid* thisBoid)
{
	Vector2f result;
	result.X = 0;
	result.Y = 0;

	for (int i = 0; i < num_boids; i++) {
		auto boid = (boids + (flockNum * num_boids) + i);

		if (boid != thisBoid) {
			auto distance = subtract(boid->Position, thisBoid->Position);

			if (norm(distance) < 3 * BOID_RADIUS) {
				result = subtract(result, distance);
			}
		}
	}

	return result;
}

Vector2f alignmentRule(Boid* boids, int flockNum, int num_boids, Boid* thisBoid)
{
	Vector2f result;
	result.X = 0;
	result.Y = 0;

	for (int i = 0; i < num_boids; i++) {
		auto boid = (boids + (flockNum * num_boids) + i);

		if (boid != thisBoid) {
			result = add(result, boid->Velocity);
		}
	}

	if (norm(result) == 0) {
		return result;
	}

	result = divide(result, (float)(num_boids - 1));

	return divide(subtract(result, thisBoid->Velocity), 8.0f);
}

Vector2f restrictPosition(const Boid& boid)
{
	Vector2f result;
	result.X = 0;
	result.Y = 0;
	float push = BOOST_SPEED;

	if (boid.Position.X < 0) {
		result.X = push;
	}
	if (boid.Position.X > WINDOW_WIDTH) {
		result.X = -push;
	}

	if (boid.Position.Y < 0) {
		result.Y = push;
	}
	if (boid.Position.Y > WINDOW_HEIGHT) {
		result.Y = -push;
	}

	return result;
}

Vector2f moveToPoint(Boid* thisBoid, Vector2f* focusPoint)
{
	Vector2f result;
	result.X = 0;
	result.Y = 0;

	auto distance = norm(subtract(thisBoid->Position, *focusPoint));

	if (distance < BOID_RADIUS * 10) {
		focusPoint->X = RNG::getNextInt(0, WINDOW_WIDTH);
		focusPoint->Y = RNG::getNextInt(0, WINDOW_HEIGHT);
	}
	else {
		result = divide(subtract(thisBoid->Position, *focusPoint), 100.0f);
	}
	
	return result;
}

void restrictVelocity(Boid* thisBoid)
{
	Vector2f result;
	result.X = 0;
	result.Y = 0;
	float vnorm = norm(thisBoid->NewVelocity);

	if (vnorm > BOOST_SPEED) {
		result = multiply(divide(thisBoid->NewVelocity, vnorm), BOOST_SPEED);

		thisBoid->NewVelocity.X = result.X;
		thisBoid->NewVelocity.Y = result.Y;
	}
}

void updateVelocity(Boid* boids, int flockNum, int num_boids, Boid* boid, Vector2f* focusPoint)
{
	auto rule1 = cohesionRule(boids, flockNum, num_boids, boid);
	auto rule2 = separationRule(boids, flockNum, num_boids, boid);
	auto rule3 = alignmentRule(boids, flockNum, num_boids, boid);
	auto rule4 = moveToPoint(boid, focusPoint);
	auto rulePos = restrictPosition(*boid);

	boid->NewVelocity = add(add(boid->Velocity, add(add(add(rule1, rule2), rule3), rulePos)), rule4);
	restrictVelocity(boid);
}