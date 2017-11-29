#include "OpenCL_Helper.h"

std::string getKernel()
{
	return R"(
__kernel void foo(__global int* x)
{
	for (int i = 0; i < 5; i++) {
		printf("%d\n", *(x + i));
	}
}
)";
}

std::string getCPUKernel()
{
	return R"(
__global void foo2()
{
	printf("Hello world!");
}
)";
}

void initOpenCL()
{
	auto devices = clhelper::getDevicesOfType(CL_DEVICE_TYPE_CPU);
	auto context = clhelper::createContext(devices);
	auto queue = clhelper::createCommandQueue(context, devices[0]);
	//auto program = clhelper::createProgramWithSource(context, getKernel());
	//clhelper::buildProgram(program, devices);
	//auto kernel = clhelper::createKernel(program, "foo");

	auto memory = clhelper::createBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * 5);

	//clhelper::setKernelArg(kernel, 0, sizeof(cl_mem), &memory);
	//clhelper::enqueueForTaskParallelism(queue, kernel);
	//
	//clhelper::waitForQueueToFinish(queue);

	auto program2 = clhelper::createProgramWithSource(context, getCPUKernel());
	clhelper::buildProgram(program2, devices);
	auto kernel2 = clhelper::createKernel(program2, "foo2");

	clhelper::enqueueForTaskParallelism(queue, kernel2);
	clhelper::waitForQueueToFinish(queue);

	clReleaseMemObject(memory);
	//clReleaseKernel(kernel);
	//clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
}

int main()
{
	initOpenCL();
	system("PAUSE");
}