#include <cuda.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <iostream>
#include "DataTypes.h"

__global__ void updateBoid(Boid* boids)
{
	int boidId = blockIdx.x * blockDim.x + threadIdx.x;
	auto boid = boids + boidId;

	boid->Velocity.X = boid->NewVelocity.X;
	boid->Velocity.Y = boid->NewVelocity.Y;

	boid->Position.X += boid->Velocity.X;
	boid->Position.Y += boid->Velocity.Y;
}

void updateFlocks(Boid* boids, int numFlocks, int numBoids)
{
	Boid* MBoids;
	cudaMalloc((void**)&MBoids, sizeof(Boid) * numFlocks * numBoids);
	cudaMemcpy(MBoids, boids, numBoids * numFlocks * sizeof(Boid), cudaMemcpyHostToDevice);
	updateBoid<<<numFlocks, numBoids>>>(MBoids);
	cudaDeviceSynchronize();
	cudaMemcpy(boids, MBoids, numBoids * numFlocks * sizeof(Boid), cudaMemcpyDeviceToHost);
	cudaFree(MBoids);
}