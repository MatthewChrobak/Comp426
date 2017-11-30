#pragma once
#include "OpenCL_Helper.h"
#include "Kernels.h"
#include "DataTypes.h"



DeviceList cpu_devices;

DeviceList gpu_devices;
cl_context gpu_context;
cl_command_queue gpu_queue;
cl_program gpu_program;
cl_kernel kernel_updateBoids;


void triggerGpuWork(Boid* boids, int numFlocks, int numBoids)
{
	auto memSize = sizeof(Boid) * numFlocks * numBoids;
	static cl_mem gpu_memory = clhelper::createBuffer(gpu_context, CL_MEM_READ_WRITE, memSize);

	clhelper::enqueueWriteBuffer(gpu_queue, gpu_memory, 0, memSize, boids);
	clhelper::setKernelArg(kernel_updateBoids, 0, sizeof(cl_mem), &gpu_memory);
	clhelper::enqueueForDataParallelism(gpu_queue, kernel_updateBoids, numFlocks, numBoids);
	clhelper::waitForQueueToFinish(gpu_queue);

	clhelper::enqueueReadBuffer(gpu_queue, gpu_memory, 0, memSize, boids);
}

void initGpuAndWork()
{
	gpu_devices = clhelper::getDevicesOfType(CL_DEVICE_TYPE_GPU);
	gpu_context = clhelper::createContext(gpu_devices);
	gpu_queue = clhelper::createCommandQueue(gpu_context, gpu_devices[0]);
	gpu_program = clhelper::createProgramWithSource(gpu_context, sourceprogram_updateBoids());
	clhelper::buildProgram(gpu_program, gpu_devices);

	kernel_updateBoids = clhelper::createKernel(gpu_program, "updateBoid");
}

void initOpenCL()
{
	initGpuAndWork();
}