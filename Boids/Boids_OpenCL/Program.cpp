//#include <GL\glew.h>
//#include <GL\freeglut.h>
//#include <string>
//#include "Flock.h"
//#include "RNG.h"
//#include <CL\cl.h>
//#include <iostream>
//#include <string>
//
//void initOpenCL()
//{
//	auto gpu_devices = getDevicesOfType(CL_DEVICE_TYPE_GPU);
//	cl_context gpu_context = clCreateContext(nullptr, gpu_devices.size(), &gpu_devices[0], nullptr, nullptr, &clError);
//	checkError();
//	cl_command_queue gpu_work = clCreateCommandQueue(gpu_context, gpu_devices[0], 0, &clError);
//	checkError();
//
//	auto cpu_devices = getDevicesOfType(CL_DEVICE_TYPE_CPU);
//	cl_context cpu_context = clCreateContext(nullptr, cpu_devices.size(), &cpu_devices[0], nullptr, nullptr, &clError);
//	checkError();
//	cl_command_queue cpu_work = clCreateCommandQueue(cpu_context, cpu_devices[0], 0, &clError);
//	checkError();
//
//
//	const char* kernel_source = getKernel();
//	const char** kernel_source_address = &kernel_source;
//	const size_t size = 1;
//	cl_program program = clCreateProgramWithSource(cpu_context, 3, kernel_source_address, &size, &clError);
//	checkError();
//
//
//	//cl_mem buffer = clCreateBuffer(cpu_context, CL_MEM_READ_WRITE, sizeof(int), nullptr, &clError);
//	//clCheckError();
//
//
//
//	clError = clBuildProgram(program, cpu_devices.size(), cpu_devices.data(), "", nullptr, nullptr);
//	checkError();
//
//	cl_kernel kernel = clCreateKernel(program, "thisisjustatest", &clError);
//	checkError();
//
//	//clError = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&buffer);
//	//clCheckError();
//
//
//	system("PAUSE");
//}
//
//void maiss()
//{
//	initOpenCL();
//}
//
//void draw()
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glClearColor(50 / 256.0f, 150 / 256.0f, 200 / 256.0f, 1);
//
//	for (int flockIndex = 0; flockIndex < Flocks.size(); flockIndex++) {
//		auto flock = Flocks.at(flockIndex);
//
//		for (int boidIndex = 0; boidIndex < flock->Boids.size(); boidIndex++) {
//			auto boid = flock->Boids.at(boidIndex);
//			auto pos = boid->Position;
//
//			glColor3b(flock->Color.R, flock->Color.G, flock->Color.B);
//			glBegin(GL_POLYGON);
//
//			glVertex2f(Space::ConvertWidth(pos.X - BOID_RADIUS), Space::ConvertHeight(pos.Y - BOID_RADIUS));
//			glVertex2f(Space::ConvertWidth(pos.X + BOID_RADIUS), Space::ConvertHeight(pos.Y - BOID_RADIUS));
//			glVertex2f(Space::ConvertWidth(pos.X + BOID_RADIUS), Space::ConvertHeight(pos.Y + BOID_RADIUS));
//			glVertex2f(Space::ConvertWidth(pos.X - BOID_RADIUS), Space::ConvertHeight(pos.Y + BOID_RADIUS));
//			glEnd();
//		}
//	}
//
//	glutSwapBuffers();
//	glutPostRedisplay();
//}
//
//void generateFlocks()
//{
//	int numFlocks = RNG::getNextInt(3, 7);
//
//	for (int i = 0; i < numFlocks; i++) {
//		auto flock = new Flock();
//
//		flock->Color.R = RNG::getNextInt(0, 256);
//		flock->Color.G = RNG::getNextInt(0, 256);
//		flock->Color.B = RNG::getNextInt(0, 256);
//
//		int numBirds = RNG::getNextInt(10, 20);
//		while (numBirds--) {
//			auto boid = new Boid();
//			boid->Position.X = (float)RNG::getNextInt(0, WINDOW_WIDTH);
//			boid->Position.Y = (float)RNG::getNextInt(0, WINDOW_HEIGHT);
//			flock->Boids.push_back(boid);
//		}
//
//		Flocks.push_back(flock);
//	}
//}
//
//void initOpenGL()
//{
//	int val = 0;
//	glutInit(&val, nullptr);
//	glutInitDisplayMode(GLUT_SINGLE);
//	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("Assignment 1: Boids");
//	glutDisplayFunc(draw);
//}
//
//int mains(int* numargs, char** args)
//{
//	generateFlocks();
//	initOpenGL();
//
//	ULONGLONG tmrGraphics = 0;
//	ULONGLONG tmrState = 0;
//	ULONGLONG tmrFrameRate = GetTickCount64();
//	ULONGLONG fps = 0;
//
//	while (true) {
//		auto tick = GetTickCount64();
//
//		if (tick > tmrGraphics) {
//			glutMainLoopEvent();
//			tmrGraphics = tick + 16;
//			fps++;
//		}
//
//		if (tick > tmrFrameRate) {
//			std::string newTitle = "Assignment 3: Boids " + std::to_string(fps);
//			glutSetWindowTitle(newTitle.c_str());
//			tmrFrameRate = tick + 1000;
//			fps = 0;
//		}
//
//		if (tick > tmrState) {
//			for (auto flock : Flocks) {
//				for (auto boid : flock->Boids) {
//					boid->updateVelocity(flock->Boids);
//				}
//				for (auto boid : flock->Boids) {
//					boid->updatePosition();
//				}
//			}
//			tmrState = tick + 33;
//		}
//	}
//}