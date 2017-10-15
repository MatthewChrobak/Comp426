#include <GL\glew.h>
#include <GL\freeglut.h>
#include <string>
#include "Flock.h"
#include "RNG.h"
#include <tbb\parallel_for_each.h>

std::vector<Flock*> Flocks;

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
			tbb::parallel_for_each(Flocks.begin(), Flocks.end(), [&](Flock* f) { f->updateBoids(); });
			tmrState = tick + 33;
		}
	}
}