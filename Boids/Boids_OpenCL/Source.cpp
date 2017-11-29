//#include <CL\cl.h>
//#include <stdlib.h>
//
//const char* cVectorAdd[] =
//{
//	"__kernel void VectorAdd(", // 20
//	"    __global const float* a,", // 25
//	"    __global const float* b,", // 25
//	"    __global float* c)", // 19
//	"{", // 1
//	"        int iGID = get_global_id(0);",
//	"        c[iGID] = a[iGID] + b[iGID];",
//	"}"
//
//	"___kernel void foo() { hello world }";
//};
//
//int mainss()
//{
//	cl_context cxMainContext;     // OpenCL context
//	cl_command_queue cqCommandQue;// OpenCL command que
//	cl_device_id* cdDevices;      // OpenCL device list    
//	cl_program cpProgram;         // OpenCL program
//	cl_kernel ckKernel;           // OpenCL kernel
//	cl_mem cmMemObjs[3];      // OpenCL memory buffer objects
//	cl_int ciErrNum = 0;
//	//  Error code var
//	size_t szGlobalWorkSize[1];//  Global # of work items
//	size_t szLocalWorkSize[1];
//	// # of Work Items in Work Group
//	size_t szParmDataBytes;
//	// byte length of parameter storage
//	size_t szKernelLength;// byte Length of kernel code
//	int iTestN = 10000;   // Length of demo test vectors
//
//	// create the OpenCL context on a GPU device
//	cxMainContext = clCreateContextFromType(0, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);
//	// get list of GPU devices associated with context
//	clGetContextInfo(cxMainContext, CL_CONTEXT_DEVICES, 0, NULL, &szParmDataBytes);
//	cdDevices = (cl_device_id*)malloc(szParmDataBytes);
//	clGetContextInfo(cxMainContext, CL_CONTEXT_DEVICES, szParmDataBytes, cdDevices, NULL);
//
//	// create a command-queue
//	cqCommandQue = clCreateCommandQueue(cxMainContext, cdDevices[0], 0, NULL);
//
//
//	// allocate the first source buffer memory object
//	cmMemObjs[0] = clCreateBuffer(cxMainContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * iTestN, srcA, NULL);
//	// allocate the second source buffer memory object
//	cmMemObjs[1] = clCreateBuffer(cxMainContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * iTestN, srcB, NULL);
//	// allocate the destination buffer memory object
//	cmMemObjs[2] = clCreateBuffer(cxMainContext, CL_MEM_WRITE_ONLY, sizeof(cl_float) * iTestN, NULL, NULL);
//
//
//	// create the program, in this case from OpenCL C 
//	cpProgram = clCreateProgramWithSource(cxMainContext, 8, cVectorAdd, NULL, &ciErrNum);
//	// build the program
//	ciErrNum = clBuildProgram(cpProgram, 0, NULL, NULL, NULL, NULL);
//	// create the kernel
//	ckKernel = clCreateKernel(cpProgram, "VectorAdd", &ciErrNum);
//	// set the kernel Argument values
//	ciErrNum = clSetKernelArg(ckKernel, 0, sizeof(cl_mem), (void*)&cmMemObjs[0]);
//	ciErrNum |= clSetKernelArg(ckKernel, 1, sizeof(cl_mem), (void*)&cmMemObjs[1]);
//	ciErrNum |= clSetKernelArg(ckKernel, 2, sizeof(cl_mem), (void*)&cmMemObjs[2]);
//
//
//
//	// set work-item dimensions
//	szGlobalWorkSize[0] = iTestN;
//	szLocalWorkSize[0] = 1;
//	// execute kernel
//	ciErrNum = clEnqueueNDRangeKernel(cqCommandQue, ckKernel, 1, NULL, szGlobalWorkSize, szLocalWorkSize, 0, NULL, NULL);
//	// read output
//	ciErrNum = clEnqueueReadBuffer(cqCommandQue,cmMemObjs[2], CL_TRUE, 0, iTestN * sizeof(cl_float), dst, 0, NULL, NULL);
//
//	// release kernel, program, and memory objects
//	DeleteMemobjs(cmMemObjs, 3);
//	free(cdDevices);
//	clReleaseKernel(ckKernel);
//	clReleaseProgram(cpProgram);
//	clReleaseCommandQueue(cqCommandQue);
//	clReleaseContext(cxMainContext);
//}
