#include "OpenCL_Helper.h"

std::string getKernel()
{
	return R"(
__kernel void foo(int x)
{
	printf("%d\n", x);
})";
}

void initOpenCL()
{
	auto devices = clhelper::getDevicesOfType(CL_DEVICE_TYPE_CPU);
	auto context = clhelper::createContext(devices);
	auto queue = clhelper::createCommandQueue(context, devices[0]);
	auto program = clhelper::createProgramWithSource(context, getKernel());
	clhelper::buildProgram(program, devices);
	auto kernel = clhelper::createKernel(program, "foo");

	int val = 101;
	clhelper::setKernelArg(kernel, 0, sizeof(val), &val);
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