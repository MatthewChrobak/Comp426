#include <cuda.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include "Boid.h"
#include <iostream>


__global__ void updateBoid(Boid** boids)
{
	int boidId = blockIdx.x * blockDim.x + threadIdx.x;
	Boid* boid = *(boids + boidId);
	
	int newpos = boid->Position.X + boid->Velocity.X;
	printf("Boids are at: %f\n", newpos);
	//boid->Position.Y += boid->Velocity.Y;
}

void updateFlocks(Boid** boids, int numFlocks, int numBoids)
{
	updateBoid<<<numFlocks, numBoids>>>(boids);
	cudaDeviceSynchronize();
}