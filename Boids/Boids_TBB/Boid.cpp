#include "Boid.h"

void Boid::updateVelocity(const std::vector<Boid*>& flock)
{
	auto rule1 = this->cohesionRule(flock);
	auto rule2 = this->separationRule(flock);
	auto rule3 = this->alignmentRule(flock);
	auto rulePos = this->restrictPosition(*this);

	Vector newVelocity = this->Velocity + rule1 + rule2 + rule3 + rulePos;

	this->Velocity += newVelocity;
	this->restrictVelocity();
}

void Boid::updatePosition()
{
	this->Position += this->Velocity;
}

Vector Boid::cohesionRule(const std::vector<Boid*>& flock)
{
	Vector result(0, 0);

	for (int i = 0; i < flock.size(); i++) {
		auto boid = flock.at(i);

		if (boid != this) {
			result = result + boid->Position;
		}
	}

	if (result.norm() == 0) {
		return result;
	}

	result = result / (float)(flock.size() - 1);
	return (result - this->Position) / 100.0f;
}

Vector Boid::separationRule(const std::vector<Boid*>& flock)
{
	Vector result(0, 0);

	for (int i = 0; i < flock.size(); i++) {
		auto boid = flock.at(i);

		if (boid != this) {
			auto distance = boid->Position - this->Position;

			if (distance.norm() < 3 * BOID_RADIUS) {
				result = result - distance;
			}
		}
	}

	return result;
}

Vector Boid::alignmentRule(const std::vector<Boid*>& flock)
{
	Vector result(0, 0);

	for (int i = 0; i < flock.size(); i++) {
		auto boid = flock.at(i);

		if (boid != this) {
			result = result + boid->Velocity;
		}
	}

	if (result.norm() == 0) {
		return result;
	}

	result = result / (float)(flock.size() - 1);

	return (result - this->Velocity) / 8.0f;
}

Vector Boid::restrictPosition(const Boid& boid)
{
	Vector result(0, 0);
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
	Vector result(0, 0);
	float norm = this->Velocity.norm();

	if (norm > BOOST_SPEED) {
		result = (this->Velocity / norm) * BOOST_SPEED;

		this->Velocity.X = result.X;
		this->Velocity.Y = result.Y;
	}
}