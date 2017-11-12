#pragma once
#include "Vector2f.h"
#include "Space.h"
#include <vector>

#define BOOST_SPEED 5
#define BOID_RADIUS 2.0f

class Boid
{
public:
	Vector2f Position;
	Vector2f Velocity;

	void updateVelocity(Boid** boids, int flockNum, int num_boids);
	void updatePosition();

private:
	Vector2f cohesionRule(Boid** boids, int flockNum, int num_boids);
	Vector2f separationRule(Boid** boids, int flockNum, int num_boids);
	Vector2f alignmentRule(Boid** boids, int flockNum, int num_boids);
	Vector2f restrictPosition(const Boid& boid);
	void restrictVelocity();
};