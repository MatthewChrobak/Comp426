#include "OpenCL_Helper.h"

std::string getKernel()
{
	return R"(
__kernel void foo()
{
	printf("Hello world!\n");
})";
}

void initOpenCL()
{
	auto cpu_devices = clhelper::getDevicesOfType(CL_DEVICE_TYPE_CPU);
	auto context = clhelper::createContext(cpu_devices);
	auto queue = clhelper::createCommandQueue(context, cpu_devices[0]);
	auto program = clhelper::createProgramWithSource(context, getKernel());
	clhelper::buildProgram(program, cpu_devices);
	auto kernel = clhelper::createKernel(program, "foo");

	clhelper::enqueueForTaskParallelism(queue, kernel);
	clhelper::waitForQueueToFinish(queue);

	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
}

int main()
{
	initOpenCL();
	system("PAUSE");
}