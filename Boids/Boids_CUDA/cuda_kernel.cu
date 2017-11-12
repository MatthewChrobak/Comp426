#include <cuda.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <iostream>
#include "DataTypes.h"

__global__ void updateBoid(Boid* boids, int numFlocks, int numBoids)
{
	int boidId = blockIdx.x * blockDim.x + threadIdx.x;
	auto boid = boids + boidId;
	
	boid->Position.X += boid->Velocity.X;
	boid->Position.Y += boid->Velocity.Y;
}

void updateFlocks(Boid* boids, int numFlocks, int numBoids)
{
	Boid* MBoids;
	cudaMalloc((void**)&MBoids, sizeof(Boid) * numFlocks * numBoids);
	Boid* localBoids = (Boid*)malloc(numFlocks * numBoids * sizeof(Boid));

	cudaMemcpy(MBoids, boids, numBoids * numFlocks * sizeof(Boid), cudaMemcpyHostToDevice);
	updateBoid<<<numFlocks, numBoids >>>(MBoids, numFlocks, numBoids);
	cudaDeviceSynchronize();
	cudaMemcpy(localBoids, MBoids, numBoids * numFlocks * sizeof(Boid), cudaMemcpyDeviceToHost);

	for (int i = 0; i < numBoids * numFlocks; i++) {
		*(boids + i) = *(localBoids + i);
	}
	free(localBoids);
	cudaFree(MBoids);
}