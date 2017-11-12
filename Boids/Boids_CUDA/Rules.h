#pragma once
#include "DataTypes.h"
#include "Space.h"
#include <math.h>
#define BOOST_SPEED 5
#define BOID_RADIUS 2.0f

Vector add(Vector v1, Vector v2)
{
	v1.X += v2.X;
	v1.Y += v2.Y;
	return v1;
}

Vector subtract(Vector v1, Vector v2)
{
	v1.X -= v2.X;
	v1.Y -= v2.Y;
	return v1;
}

Vector multiply(Vector v1, float scalar)
{
	v1.X *= scalar;
	v1.Y *= scalar;
	return v1;
}

Vector divide(Vector v1, float scalar)
{
	v1.X /= scalar;
	v1.Y /= scalar;
	return v1;
}

float norm(Vector v)
{
	return sqrt(v.X * v.X + v.Y * v.Y);
}

void updatePosition(Boid* boid)
{
	boid->Position = add(boid->Position, boid->Velocity);
}

Vector cohesionRule(Boid* boids, int flockNum, int num_boids, Boid* thisBoid)
{
	Vector result;
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

Vector separationRule(Boid* boids, int flockNum, int num_boids, Boid* thisBoid)
{
	Vector result;
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

Vector alignmentRule(Boid* boids, int flockNum, int num_boids, Boid* thisBoid)
{
	Vector result;
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

Vector restrictPosition(const Boid& boid)
{
	Vector result;
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

void restrictVelocity(Boid* thisBoid)
{
	Vector result;
	result.X = 0;
	result.Y = 0;
	float vnorm = norm(thisBoid->Velocity);

	if (vnorm > BOOST_SPEED) {
		result = multiply(divide(thisBoid->Velocity, vnorm), BOOST_SPEED);

		thisBoid->Velocity.X = result.X;
		thisBoid->Velocity.Y = result.Y;
	}
}

void updateVelocity(Boid* boids, int flockNum, int num_boids, Boid* boid)
{
	auto rule1 = cohesionRule(boids, flockNum, num_boids, boid);
	auto rule2 = separationRule(boids, flockNum, num_boids, boid);
	auto rule3 = alignmentRule(boids, flockNum, num_boids, boid);
	auto rulePos = restrictPosition(*boid);

	auto newVelocity = add(add(add(add(boid->Velocity, rule1), rule2), rule3), rulePos);

	boid->Velocity = add(boid->Velocity, newVelocity);
	restrictVelocity(boid);
}