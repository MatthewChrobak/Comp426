#include "Boid.h"

void Boid::updateVelocity(Boid** boids, int flockNum, int num_boids)
{
	auto rule1 = this->cohesionRule(boids, flockNum, num_boids);
	auto rule2 = this->separationRule(boids, flockNum, num_boids);
	auto rule3 = this->alignmentRule(boids, flockNum, num_boids);
	auto rulePos = this->restrictPosition(*this);

	Vector2f newVelocity = this->Velocity + rule1 + rule2 + rule3 + rulePos;

	this->Velocity += newVelocity;
	this->restrictVelocity();
}

void Boid::updatePosition()
{
	this->Position += this->Velocity;
}

Vector2f Boid::cohesionRule(Boid** boids, int flockNum, int num_boids)
{
	Vector2f result(0, 0);

	for (int i = 0; i < num_boids; i++) {
		auto boid = *(boids + (flockNum * num_boids) + i);

		if (boid != this) {
			result = result + boid->Position;
		}
	}

	if (result.norm() == 0) {
		return result;
	}

	result = result / (float)(num_boids - 1);
	return (result - this->Position) / 100.0f;
}

Vector2f Boid::separationRule(Boid** boids, int flockNum, int num_boids)
{
	Vector2f result(0, 0);

	for (int i = 0; i < num_boids; i++) {
		auto boid = *(boids + (flockNum * num_boids) + i);

		if (boid != this) {
			auto distance = boid->Position - this->Position;

			if (distance.norm() < 3 * BOID_RADIUS) {
				result = result - distance;
			}
		}
	}

	return result;
}

Vector2f Boid::alignmentRule(Boid** boids, int flockNum, int num_boids)
{
	Vector2f result(0, 0);

	for (int i = 0; i < num_boids; i++) {
		auto boid = *(boids + (flockNum * num_boids) + i);

		if (boid != this) {
			result = result + boid->Velocity;
		}
	}

	if (result.norm() == 0) {
		return result;
	}

	result = result / (float)(num_boids - 1);

	return (result - this->Velocity) / 8.0f;
}

Vector2f Boid::restrictPosition(const Boid& boid)
{
	Vector2f result(0, 0);
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

void Boid::restrictVelocity()
{
	Vector2f result(0, 0);
	float norm = this->Velocity.norm();

	if (norm > BOOST_SPEED) {
		result = (this->Velocity / norm) * BOOST_SPEED;

		this->Velocity.X = result.X;
		this->Velocity.Y = result.Y;
	}
}