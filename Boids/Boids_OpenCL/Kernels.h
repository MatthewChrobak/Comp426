#include <string>

std::string sourceprogram_updateBoids()
{
	return R"(
#include "DataTypes.h"

__kernel void updateBoid(__global Boid* boids)
{
	int boidId = get_global_id(0);
	__global Boid* boid = boids + boidId;

	boid->Position.X += boid->Velocity.X;
	boid->Position.Y += boid->Velocity.Y;

	//printf("Updating boid %d \n", boidId);
}
)";
}

std::string getRules()
{
	return R"(
#include "DataTypes.h"

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 900
#define BOOST_SPEED 10
#define BOID_RADIUS 2.0f
#define DISTANCE (20 * BOID_RADIUS)

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

void updatePosition(__global Boid* boid)
{
	boid->Position = add(boid->Position, boid->Velocity);
}

bool isCloseby(__global Boid* boid1, __global Boid* boid2)
{
	return norm(subtract(boid1->Position, boid2->Position)) <= DISTANCE;
}

Vector cohesionRule(__global Boid* boids, int flockNum, int num_boids, __global Boid* thisBoid)
{
	Vector result;
	result.X = 0;
	result.Y = 0;
	int totalSeen = 0;

	for (int i = 0; i < num_boids; i++) {
		__global Boid* boid = (boids + (flockNum * num_boids) + i);

		if (boid != thisBoid && isCloseby(thisBoid, boid)) {
			result = add(result, boid->Position);
			totalSeen++;
		}
	}

	if (norm(result) == 0) {
		return result;
	}

	if (totalSeen == 1) {
		totalSeen = 2;
	}

	result = divide(result, (float)(totalSeen - 1));
	return divide(subtract(result, thisBoid->Position), 100.0f);
}

Vector separationRule(__global Boid* boids, int flockNum, int num_boids, __global Boid* thisBoid)
{
	Vector result;
	result.X = 0;
	result.Y = 0;

	for (int i = 0; i < num_boids; i++) {
		__global Boid* boid = (boids + (flockNum * num_boids) + i);

		if (boid != thisBoid) {
			Vector distance = subtract(boid->Position, thisBoid->Position);


			if (norm(distance) < 5 * BOID_RADIUS && isCloseby(thisBoid, boid)) {
				result = subtract(result, distance);
			}
		}
	}

	return result;
}

Vector alignmentRule(__global Boid* boids, int flockNum, int num_boids, __global Boid* thisBoid)
{
	Vector result;
	result.X = 0;
	result.Y = 0;

	int totalSeen = 0;

	for (int i = 0; i < num_boids; i++) {
		__global Boid* boid = (boids + (flockNum * num_boids) + i);

		if (boid != thisBoid && isCloseby(thisBoid, boid)) {
			result = add(result, boid->Velocity);
			totalSeen++;
		}
	}

	if (norm(result) == 0) {
		return result;
	}

	if (totalSeen == 1) {
		totalSeen = 2;
	}

	result = divide(result, (float)(totalSeen - 1));

	return divide(subtract(result, thisBoid->Velocity), 8.0f);
}

Vector restrictPosition(__global Boid* boid)
{
	Vector result;
	result.X = 0;
	result.Y = 0;

	boid->Position.X = fmod(boid->Position.X, WINDOW_WIDTH);
	boid->Position.Y = fmod(boid->Position.Y, WINDOW_HEIGHT);

	return result;
}

void restrictVelocity(__global Boid* thisBoid)
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

void updateVelocity(__global Boid* boids, int flockNum, int num_boids, __global Boid* boid)
{
	Vector rule1 = cohesionRule(boids, flockNum, num_boids, boid);
	Vector rule2 = separationRule(boids, flockNum, num_boids, boid);
	Vector rule3 = alignmentRule(boids, flockNum, num_boids, boid);
	Vector rulePos = restrictPosition(boid);

	Vector newVelocity = add(add(add(rule1, rule2), rule3), rulePos);

	boid->Velocity = add(boid->Velocity, newVelocity);
	restrictVelocity(boid);
}
)";
}

std::string sourceprogram_updateFlock()
{
	return getRules() + R"(
__kernel void updateFlock(__global Boid* boids, int numBoidsPerFlock)
{
	int flockIndex = get_global_id(0);

	for (int i = 0; i < numBoidsPerFlock; i++) {
		__global Boid* thisBoid = boids + numBoidsPerFlock * flockIndex + i;
		updateVelocity(boids, flockIndex, numBoidsPerFlock, thisBoid);
	}
}
)";
}