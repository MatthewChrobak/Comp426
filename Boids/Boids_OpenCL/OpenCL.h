#pragma once
#include "OpenCL_Helper.h"
#include "Kernels.h"
#include "DataTypes.h"

void triggerGpuWork(Boid* boids, int numFlocks, int numBoidsPerFlock)
{
	static auto devices = clhelper::getDevicesOfType(CL_DEVICE_TYPE_GPU);
	static auto context = clhelper::createContext(devices);
	static auto queue = clhelper::createCommandQueue(context, devices[0]);
	static auto program = clhelper::createProgramWithSource(context, sourceprogram_updateBoids());
	static auto program_built = clhelper::buildProgram(program, devices);
	static auto kernel_updateBoids = clhelper::createKernel(program, "updateBoid");

	auto memSize = sizeof(Boid) * numFlocks * numBoidsPerFlock;
	static auto memory = clhelper::createBuffer(context, CL_MEM_READ_WRITE, memSize);
	static auto kernelparam_set = clhelper::setKernelArg(kernel_updateBoids, 0, sizeof(cl_mem), &memory);


	clhelper::enqueueWriteBuffer(queue, memory, 0, memSize, boids);
	clhelper::waitForQueueToFinish(queue);

	clhelper::enqueueForDataParallelism(queue, kernel_updateBoids, numFlocks, numBoidsPerFlock);
	clhelper::waitForQueueToFinish(queue);

	clhelper::enqueueReadBuffer(queue, memory, 0, memSize, boids);
	clhelper::waitForQueueToFinish(queue);
}

void triggerCpuWork(Boid* boids, int numFlocks, int numBoidsPerFlock)
{
	static auto devices = clhelper::getDevicesOfType(CL_DEVICE_TYPE_CPU);
	static auto context = clhelper::createContext(devices);
	static auto queue = clhelper::createCommandQueue(context, devices[0]);
	static auto program = clhelper::createProgramWithSource(context, sourceprogram_updateFlock());
	static auto program_built = clhelper::buildProgram(program, devices);
	static auto kernel_updateFlock = clhelper::createKernel(program, "updateFlock");

	auto memSize = sizeof(Boid) * numFlocks * numBoidsPerFlock;
	static auto memory = clhelper::createBuffer(context, CL_MEM_READ_WRITE, memSize);
	static auto kernelparam_set_mem = clhelper::setKernelArg(kernel_updateFlock, 0, sizeof(cl_mem), &memory);
	static auto kernelparam_set_numBoids = clhelper::setKernelArg(kernel_updateFlock, 1, sizeof(int), &numBoidsPerFlock);

	clhelper::enqueueWriteBuffer(queue, memory, 0, memSize, boids);
	clhelper::waitForQueueToFinish(queue);

	clhelper::enqueueForDataParallelism(queue, kernel_updateFlock, numFlocks, 1);
	clhelper::waitForQueueToFinish(queue);

	clhelper::enqueueReadBuffer(queue, memory, 0, memSize, boids);
	clhelper::waitForQueueToFinish(queue);
}