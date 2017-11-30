#include <GL\glew.h>
#include <GL\freeglut.h>
#include <string>

#include "Threads.h"

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(50 / 256.0f, 150 / 256.0f, 200 / 256.0f, 1);

	for (int flockIndex = 0; flockIndex < Flocks.size(); flockIndex++) {
		auto flock = Flocks.at(flockIndex);

		for (int boidIndex = 0; boidIndex < flock->Boids.size(); boidIndex++) {
			auto boid = flock->Boids.at(boidIndex);
			auto pos = boid->CurPos;

			glColor3b(flock->R, flock->G, flock->B);
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

void generateFlockThreads()
{
	for (int i = 0; i < Flocks.size(); i++) {
		new std::thread(flockThread, i);
	}
}

void generateFlocks()
{
	int numFlocks = RNG::getNextInt(3, 7);

	for (int i = 0; i < numFlocks; i++) {
		auto flock = new Flock();

		flock->R = RNG::getNextInt(0, 256);
		flock->G = RNG::getNextInt(0, 256);
		flock->B = RNG::getNextInt(0, 256);

		int numBirds = RNG::getNextInt(10, 20);
		while (numBirds--) {
			auto boid = new Boid();
			boid->CurPos.X = (float)RNG::getNextInt(0, WINDOW_WIDTH);
			boid->CurPos.Y = (float)RNG::getNextInt(0, WINDOW_HEIGHT);
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


void foo()
{
	static int x = 10;
	x += 10;
	printf("%d", x);
}

int main(int* numargs, char** args)
{
	foo();
	foo();
	foo();
	system("PAUSE");


	return 0;
	generateFlocks();
	initOpenGL();
	generateFlockThreads();

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
			for (int i = 0; i < Flocks.size(); i++) {
				Flocks.at(i)->RequiresLogicUpdate = true;
			}
			tmrState = tick + 33;
		}	
	}
}