#pragma once
#include "Boid.h"
#include "RNG.h"

#include <thread>
#include <vector>
#include "Vector2f.h"
#include "Space.h"
#include "Flock.h"

#define BOOST_SPEED 5
#define BOID_RADIUS 2.0f

std::vector<Flock*> Flocks;

Vector2f cohesionRule(int flockIndex, int boidIndex)
{
	auto flock = &Flocks.at(flockIndex)->Boids;
	Vector2f result(0, 0);

	for (int i = 0; i < flock->size(); i++) {
		if (i != boidIndex) {
			result = result + flock->at(i)->CurPos;
		}
	}

	if (result.norm() == 0) {
		return result;
	}

	result = result / (float)(flock->size() - 1);
	return (result - flock->at(boidIndex)->CurPos) / 100.0f;
}

Vector2f separationRule(int flockIndex, int boidIndex)
{
	auto flock = &Flocks.at(flockIndex)->Boids;
	Vector2f result(0, 0);

	for (int i = 0; i < flock->size(); i++) {
		if (i != boidIndex) {
			auto distance = flock->at(i)->CurPos - flock->at(boidIndex)->CurPos;

			if (distance.norm() < 3 * BOID_RADIUS) {
				result = result - distance;
			}
		}
	}

	return result;
}

Vector2f alignmentRule(int flockIndex, int boidIndex)
{
	auto flock = &Flocks.at(flockIndex)->Boids;
	Vector2f result(0, 0);

	for (int i = 0; i < flock->size(); i++) {
		if (i != boidIndex) {
			result = result + flock->at(i)->Velocity;
		}
	}

	if (result.norm() == 0) {
		return result;
	}

	result = result / (float)(flock->size() - 1);

	return (result - flock->at(boidIndex)->Velocity) / 8.0f;
}

void restrictVelocity(int flockIndex, int boidIndex)
{
	Vector2f result(0, 0);
	auto boid = Flocks.at(flockIndex)->Boids.at(boidIndex);
	float norm = boid->Velocity.norm();

	if (norm > BOOST_SPEED) {
		result = (boid->Velocity / norm) * BOOST_SPEED;

		boid->Velocity.X = result.X;
		boid->Velocity.Y = result.Y;
	}
}

Vector2f restrictPos(int flockIndex, int boidIndex)
{
	Vector2f result(0, 0);
	auto p = Flocks.at(flockIndex)->Boids.at(boidIndex)->CurPos;
	float push = BOOST_SPEED;

	if (p.X < 0) {
		result.X = push;
	}
	if (p.X > WINDOW_WIDTH) {
		result.X = -push;
	}

	if (p.Y < 0) {
		result.Y = push;
	}
	if (p.Y > WINDOW_HEIGHT) {
		result.Y = -push;
	}

	return result;
}