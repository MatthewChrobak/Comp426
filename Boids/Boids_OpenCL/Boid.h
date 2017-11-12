#pragma once
#include "Vector2f.h"
#include "Space.h"
#include <vector>

#define BOOST_SPEED 5
#define BOID_RADIUS 2.0f

class Boid
{
public:
	Vector Position;
	Vector Velocity;

	void updateVelocity(const std::vector<Boid*>& flock);
	void updatePosition();

private:
	Vector cohesionRule(const std::vector<Boid*>& flock);
	Vector separationRule(const std::vector<Boid*>& flock);
	Vector alignmentRule(const std::vector<Boid*>& flock);
	Vector restrictPosition(const Boid& flock);
	void restrictVelocity();
};