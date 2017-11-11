#include <GL\glew.h>
#include <GL\freeglut.h>
#include <string>
#include "Flock.h"
#include "RNG.h"
#include <CL\cl.h>
#include <iostream>
#include <string>


std::vector<Flock*> Flocks;
cl_int clError;

void clCheckError()
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
	clCheckError();

	char* cstr = (char*)malloc(valueSize);
	clError = clGetPlatformInfo(platform, type, valueSize, cstr, nullptr);
	clCheckError();

	std::string info(cstr);
	free(cstr);

	return info;
}

std::string getDeviceInfo(cl_device_id device, cl_device_info type)
{
	size_t valueSize = 0;
	clError = clGetDeviceInfo(device, type, 0, nullptr, &valueSize);
	clCheckError();

	char* cstr = (char*)malloc(valueSize);
	clError = clGetDeviceInfo(device, type, valueSize, cstr, nullptr);
	clCheckError();

	std::string info(cstr);
	free(cstr);

	return info;
}

std::vector<cl_platform_id> clGetPlatforms()
{
	cl_uint numPlatforms = 0;
	clError = clGetPlatformIDs(0, nullptr, &numPlatforms);
	clCheckError();

	std::cout << "Found " << numPlatforms << " platforms." << std::endl;

	std::vector<cl_platform_id> platforms(numPlatforms);
	std::vector<cl_platform_id> opencl_platforms;
	clError = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
	clCheckError();

	for (auto platform : platforms)
	{
		std::string name = getPlatformInfo(platform, CL_PLATFORM_NAME);

		if (name.find("OpenCL") != std::string::npos) {
			std::cout << "Found platform " << name << " at " << platform << std::endl;
			opencl_platforms.push_back(platform);
		}
	}

	return opencl_platforms;
}

std::vector<cl_device_id> clGetDevicesOfType(cl_int type)
{
	auto platforms = clGetPlatforms();
	std::vector<cl_device_id> devices;

	for (auto platform : platforms)
	{
		cl_uint numDevices = 0;
		clError = clGetDeviceIDs(platforms[0], type, 0, nullptr, &numDevices);
		clCheckError();

		std::cout << "Found " << numDevices << " devices." << std::endl;

		std::vector<cl_device_id> platformDevices(numDevices);
		clError = clGetDeviceIDs(platforms[0], type, numDevices, platformDevices.data(), nullptr);
		clCheckError();

		for (auto device : platformDevices)
		{
			std::string name = getDeviceInfo(device, CL_DEVICE_NAME);
			std::cout << "Found device " << name << " at " << device << std::endl;

			devices.push_back(device);
		}
	}

	return devices;
}

void initOpenCL()
{
	auto devices = clGetDevicesOfType(CL_DEVICE_TYPE_GPU);

	cl_uint num = devices.size();
	cl_context context = clCreateContext(nullptr, num, &devices[0], nullptr, nullptr, &clError);
	clCheckError();

	
	cl_command_queue queue = clCreateCommandQueue(context, devices[0], 0, &clError);
	clCheckError();
	
	system("PAUSE");
}

void main()
{
	initOpenCL();
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(50 / 256.0f, 150 / 256.0f, 200 / 256.0f, 1);

	for (int flockIndex = 0; flockIndex < Flocks.size(); flockIndex++) {
		auto flock = Flocks.at(flockIndex);

		for (int boidIndex = 0; boidIndex < flock->Boids.size(); boidIndex++) {
			auto boid = flock->Boids.at(boidIndex);
			auto pos = boid->Position;

			glColor3b(flock->Color.R, flock->Color.G, flock->Color.B);
			glBegin(GL_POLYGON);

			glVertex2f(Space::ConvertWidth(pos.X - BOID_RADIUS), Space::ConvertHeight(pos.Y - BOID_RADIUS));
			glVertex2f(Space::ConvertWidth(pos.X + BOID_RADIUS), Space::ConvertHeight(pos.Y - BOID_RADIUS));
			glVertex2f(Space::ConvertWidth(pos.X + BOID_RADIUS), Space::ConvertHeight(pos.Y + BOID_RADIUS));
			glVertex2f(Space::ConvertWidth(pos.X - BOID_RADIUS), Space::ConvertHeight(pos.Y + BOID_RADIUS));
			glEnd();
		}
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void generateFlocks()
{
	int numFlocks = RNG::getNextInt(3, 7);

	for (int i = 0; i < numFlocks; i++) {
		auto flock = new Flock();

		flock->Color.R = RNG::getNextInt(0, 256);
		flock->Color.G = RNG::getNextInt(0, 256);
		flock->Color.B = RNG::getNextInt(0, 256);

		int numBirds = RNG::getNextInt(10, 20);
		while (numBirds--) {
			auto boid = new Boid();
			boid->Position.X = (float)RNG::getNextInt(0, WINDOW_WIDTH);
			boid->Position.Y = (float)RNG::getNextInt(0, WINDOW_HEIGHT);
			flock->Boids.push_back(boid);
		}

		Flocks.push_back(flock);
	}
}

void initOpenGL()
{
	int val = 0;
	glutInit(&val, nullptr);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Assignment 1: Boids");
	glutDisplayFunc(draw);
}

int mains(int* numargs, char** args)
{
	generateFlocks();
	initOpenGL();

	ULONGLONG tmrGraphics = 0;
	ULONGLONG tmrState = 0;
	ULONGLONG tmrFrameRate = GetTickCount64();
	ULONGLONG fps = 0;

	while (true) {
		auto tick = GetTickCount64();

		if (tick > tmrGraphics) {
			glutMainLoopEvent();
			tmrGraphics = tick + 16;
			fps++;
		}

		if (tick > tmrFrameRate) {
			std::string newTitle = "Assignment 3: Boids " + std::to_string(fps);
			glutSetWindowTitle(newTitle.c_str());
			tmrFrameRate = tick + 1000;
			fps = 0;
		}

		if (tick > tmrState) {
			for (auto flock : Flocks) {
				for (auto boid : flock->Boids) {
					boid->updateVelocity(flock->Boids);
				}
				for (auto boid : flock->Boids) {
					boid->updatePosition();
				}
			}
			tmrState = tick + 33;
		}
	}
}