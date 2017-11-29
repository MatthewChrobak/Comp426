#include <CL\cl.h>
#include <iostream>
#include <vector>
#include <string>

namespace clhelper {

	cl_int clError;
	
	void checkForCLError()
	{
		if (clError != CL_SUCCESS) {
			std::cout << "CL ERROR: " << clError << std::endl;
			throw;
		}
	}

	std::string getPlatformInfo(cl_platform_id platform, cl_platform_info type)
	{
		size_t valueSize = 0;
		clError = clGetPlatformInfo(platform, type, 0, nullptr, &valueSize);
		checkForCLError();

		char* cstr = (char*)malloc(valueSize);
		clError = clGetPlatformInfo(platform, type, valueSize, cstr, nullptr);
		checkForCLError();

		std::string info(cstr);
		free(cstr);

		return info;
	}

	std::vector<cl_platform_id> getPlatforms()
	{
		cl_uint numPlatforms = 0;
		clError = clGetPlatformIDs(0, nullptr, &numPlatforms);
		checkForCLError();

		std::vector<cl_platform_id> platforms(numPlatforms);
		std::vector<cl_platform_id> opencl_platforms;
		clError = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
		checkForCLError();

		for (auto platform : platforms)
		{
			std::string name = getPlatformInfo(platform, CL_PLATFORM_NAME);

			if (name.find("OpenCL") != std::string::npos) {
				opencl_platforms.push_back(platform);
			}
		}

		return opencl_platforms;
	}

	std::string getDeviceInfo(cl_device_id device, cl_device_info type)
	{
		size_t valueSize = 0;
		clError = clGetDeviceInfo(device, type, 0, nullptr, &valueSize);
		checkForCLError();

		char* cstr = (char*)malloc(valueSize);
		clError = clGetDeviceInfo(device, type, valueSize, cstr, nullptr);
		checkForCLError();

		std::string info(cstr);
		free(cstr);

		return info;
	}

	std::vector<cl_device_id> getDevicesOfType(cl_int type)
	{
		auto platforms = getPlatforms();
		std::vector<cl_device_id> devices;

		for (auto platform : platforms)
		{
			cl_uint numDevices = 0;
			clError = clGetDeviceIDs(platforms[0], type, 0, nullptr, &numDevices);
			checkForCLError();

			std::vector<cl_device_id> platformDevices(numDevices);
			clError = clGetDeviceIDs(platforms[0], type, numDevices, platformDevices.data(), nullptr);
			checkForCLError();

			for (auto device : platformDevices)
			{
				std::string name = getDeviceInfo(device, CL_DEVICE_NAME);
				std::cout << "Found device " << name << " at " << device << std::endl;

				devices.push_back(device);
			}
		}

		return devices;
	}

	cl_context createContext(std::vector<cl_device_id> devices)
	{
		auto context = clCreateContext(nullptr, devices.size(), devices.data(), nullptr, nullptr, &clError);
		checkForCLError();
		return context;
	}

	cl_command_queue createCommandQueue(cl_context context, cl_device_id deviceId)
	{
		auto queue = clCreateCommandQueue(context, deviceId, 0, &clError);
		checkForCLError();
		return queue;
	}

	cl_program createProgramWithSource(cl_context context, std::string kernelCode)
	{
		const size_t size = kernelCode.size();
		const char* cPtr = kernelCode.c_str();
		cl_program program = clCreateProgramWithSource(context, 1, &cPtr, &size, &clError);
		checkForCLError();
		return program;
	}

	void buildProgram(cl_program program, std::vector<cl_device_id> devices)
	{
		clError = clBuildProgram(program, devices.size(), devices.data(), NULL, NULL, NULL);
		checkForCLError();
	}

	cl_kernel createKernel(cl_program program, std::string functionName)
	{
		cl_kernel kernel = clCreateKernel(program, functionName.c_str(), &clError);
		checkForCLError();
		return kernel;
	}

	void enqueueForTaskParallelism(cl_command_queue queue, cl_kernel kernel)
	{
		// Source: https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clEnqueueTask.html
		// clEnqueueTask is equivalent to calling clEnqueueNDRangeKernel with 
		// work_dim = 1, global_work_offset = NULL, global_work_size[0] set to 1, and local_work_size[0] set to 1.
		size_t global_work_size = 1;
		size_t local_work_size = 1;
		clError = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);
		checkForCLError();
	}

	void enqueueForDataParallelism(cl_command_queue queue, cl_kernel kernel)
	{

	}

	void waitForQueueToFinish(cl_command_queue queue)
	{
		clError = clFinish(queue);
		checkForCLError();
	}

	cl_mem createBuffer(cl_context context, cl_mem_flags type, size_t memorySize)
	{
		cl_mem buffer = clCreateBuffer(context, type, memorySize, NULL, &clError);
		checkForCLError();
		return buffer;
	}

	void setKernelArg(cl_kernel kernel, cl_uint paramIndex, size_t sizeofParam, const void* paramValue)
	{
		clError = clSetKernelArg(kernel, paramIndex, sizeofParam, paramValue);
		checkForCLError();
	}
}