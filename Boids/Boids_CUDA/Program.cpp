#include <GL\glew.h>
#include <GL\freeglut.h>
#include <string>
#include "DataTypes.h"
#include "RNG.h"
#include "Color.h"
#include <iostream>
#include "Rules.h"
#include <cuda.h>
#include <cuda_runtime.h>

Boid* Boids;
Color* Colors;
int NUM_FLOCKS = 0;
int NUM_BOIDS = 0;

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(50 / 256.0f, 150 / 256.0f, 200 / 256.0f, 1);
	
	for (int flockIndex = 0; flockIndex < NUM_FLOCKS; flockIndex++) {
		for (int boidIndex = 0; boidIndex < NUM_BOIDS; boidIndex++) {
			auto boid = Boids + (NUM_BOIDS * flockIndex) + boidIndex;
			auto pos = boid->Position;

			auto color = *(Colors + flockIndex);
			glColor3b(color.R, color.G, color.B);
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
	NUM_FLOCKS = RNG::getNextInt(3, 7);
	NUM_BOIDS = RNG::getNextInt(10, 20);

	Boids = (Boid*)malloc(sizeof(Boid) * NUM_FLOCKS * NUM_BOIDS);
	Colors = (Color*)malloc(sizeof(Color) * NUM_FLOCKS);

	for (int i = 0; i < NUM_FLOCKS * NUM_BOIDS; i++) {
		Boid boid;
		boid.Position.X = (float)RNG::getNextInt(0, WINDOW_WIDTH);
		boid.Position.Y = (float)RNG::getNextInt(0, WINDOW_HEIGHT);

		*(Boids + i) = boid;
	}

	for (int i = 0; i < NUM_FLOCKS; i++) {
		auto color = (Colors + i);
		color->R = RNG::getNextInt(0, 256);
		color->G = RNG::getNextInt(0, 256);
		color->B = RNG::getNextInt(0, 256);
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




extern void updateFlocks(Boid* boids, int numFlocks, int numBoids);

void updateBoids(Boid* boids, int flockNum)
{
	for (int boidIndex = flockNum * NUM_BOIDS; boidIndex < (flockNum + 1) * NUM_BOIDS; boidIndex++) {
		auto boid = (boids + boidIndex);
		updateVelocity(boids, flockNum, NUM_BOIDS, boid);
	}
}

int main(int* numargs, char** args)
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
			std::string newTitle = "Assignment 1: Boids " + std::to_string(fps);
			glutSetWindowTitle(newTitle.c_str());
			tmrFrameRate = tick + 1000;
			fps = 0;
		}

		if (tick > tmrState) {
			for (int flockIndex = 0; flockIndex < NUM_FLOCKS; flockIndex++) {
				updateBoids(Boids, flockIndex);
			}
			updateFlocks(Boids, NUM_FLOCKS, NUM_BOIDS);
			tmrState = tick + 33;
		}
	}
}
