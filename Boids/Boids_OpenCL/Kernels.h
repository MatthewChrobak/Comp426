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