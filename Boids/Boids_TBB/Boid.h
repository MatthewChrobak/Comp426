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

	void updateVelocity(const std::vector<Boid*>& flock);
	void updatePosition();

private:
	Vector2f cohesionRule(const std::vector<Boid*>& flock);
	Vector2f separationRule(const std::vector<Boid*>& flock);
	Vector2f alignmentRule(const std::vector<Boid*>& flock);
	Vector2f restrictPosition(const Boid& flock);
	void restrictVelocity();
};