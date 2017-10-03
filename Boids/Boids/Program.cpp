#include <GL\glew.h>
#include <GL\freeglut.h>
#include "Boid.h"
#include "RNG.h"
#include "Rules.h"

#include <thread>
#include <mutex>
#include <vector>
#include "BoidLock.h"
#include <string>
#include "Vector2f.h"
#include <iostream>
#include "Space.h"
#include "Flock.h"

#pragma comment(linker, "/STACK:100000")

#define BOOST_SPEED 2.5f

std::vector<Flock*> Flocks;

BoidLock Cond;
int TotalBoids = 0;
bool AppRunning = true;

Vector2f Rule1(int flockIndex, int boidIndex)
{
	auto flock = &Flocks.at(flockIndex)->Boids;
	Vector2f result(0, 0);

	for (int i = 0; i < flock->size(); i++) {
		if (i != boidIndex) {
			result = result + flock->at(i)->CurPos;
		}
	}

	if (result.norm() == 0) {
		return result;
	}

	result = result / (float)(flock->size() - 1);
	return (result - flock->at(boidIndex)->CurPos) / 100.0f;
}

Vector2f Rule2(int flockIndex, int boidIndex)
{
	auto flock = &Flocks.at(flockIndex)->Boids;
	Vector2f result(0, 0);

	for (int i = 0; i < flock->size(); i++) {
		if (i != boidIndex) {
			auto distance = flock->at(i)->CurPos - flock->at(boidIndex)->CurPos;

			if (distance.norm() < BOOST_SPEED) {
				result = result - distance;
			}
		}
	}

	return result;
}

Vector2f Rule3(int flockIndex, int boidIndex)
{
	auto flock = &Flocks.at(flockIndex)->Boids;
	Vector2f result(0, 0);

	for (int i = 0; i < flock->size(); i++) {
		if (i != boidIndex) {
			result = result + flock->at(i)->Velocity;
		}
	}

	if (result.norm() == 0) {
		return result;
	}

	result = result / (float)(flock->size() - 1);

	return (result - flock->at(boidIndex)->Velocity) / 8.0f;
}

Vector2f Rule4(int flockIndex, int boidIndex)
{
	auto flock = Flocks.at(flockIndex);
	auto distanceV = flock->FocusPoint - Flocks.at(flockIndex)->Boids.at(boidIndex)->CurPos;
	
	if (distanceV.norm() < 10 * BOOST_SPEED) {
		flock->FocusPoint.X = RNG::getNextInt(0, WINDOW_WIDTH) + 10 * BOOST_SPEED;
		flock->FocusPoint.Y = RNG::getNextInt(0, WINDOW_HEIGHT) + 10 * BOOST_SPEED;
	}

	return (flock->FocusPoint - Flocks.at(flockIndex)->Boids.at(boidIndex)->CurPos) / 100.0f;
}

void restrictVelocity(int flockIndex, int boidIndex)
{
	Vector2f result(0, 0);
	auto boid = Flocks.at(flockIndex)->Boids.at(boidIndex);
	float norm = boid->Velocity.norm();

	if (norm > BOOST_SPEED) {
		result = (boid->Velocity / norm) * BOOST_SPEED;

		boid->Velocity.X = result.X;
		boid->Velocity.Y = result.Y;
	}
}

Vector2f restrictPos(int flockIndex, int boidIndex)
{
	Vector2f result(0, 0);
	auto p = Flocks.at(flockIndex)->Boids.at(boidIndex)->CurPos;
	float push = BOOST_SPEED;

	if (p.X < 0) {
		result.X = push;
	}
	if (p.X > WINDOW_WIDTH) {
		result.X = -push;
	}

	if (p.Y < 0) {
		result.Y = push;
	}
	if (p.Y > WINDOW_HEIGHT) {
		result.Y = -push;
	}

	return result;
}

void boidThread(int flockIndex, int boidIndex)
{
	Cond.lock();
	bool ourState = !Cond.State;
	Cond.unlock();

	// Get our boid here.

	while (AppRunning) {

		// Wait until we can go.
		while (true) {
			Cond.lock();
			if (Cond.State != ourState) {
				ourState = Cond.State;
				break;
			}
			Cond.unlock();
			std::this_thread::yield();
		}
		Cond.unlock();	

		auto boid = Flocks.at(flockIndex)->Boids.at(boidIndex);

		auto rule1 = Rule1(flockIndex, boidIndex);
		auto rule2 = Rule2(flockIndex, boidIndex);
		auto rule3 = Rule3(flockIndex, boidIndex);
		auto rule4 = Rule4(flockIndex, boidIndex);
		auto rulePos = restrictPos(flockIndex, boidIndex);

		Vector2f newVelocity = boid->Velocity + rule1 + rule2 + rule3 + rule4 + rulePos;

		boid->Velocity = newVelocity;
		restrictVelocity(flockIndex, boidIndex);

		boid->NewPos = boid->CurPos + boid->Velocity;


		// Let the COND know that we're done.
		Cond.lock();
		Cond.Count++;
		Cond.unlock();
	}
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(50 / 256.0f, 150 / 256.0f, 200 / 256.0f, 1);

	float degree = 1;

	for (int flockIndex = 0; flockIndex < Flocks.size(); flockIndex++) {
		auto flock = Flocks.at(flockIndex);

		for (int boidIndex = 0; boidIndex < flock->Boids.size(); boidIndex++) {
			auto boid = flock->Boids.at(boidIndex);
			auto pos = boid->CurPos;

			glColor3b(flock->R, flock->G, flock->B);
			glBegin(GL_POLYGON);
			
			glVertex2f(Space::ConvertWidth(pos.X - degree), Space::ConvertHeight(pos.Y - degree));
			glVertex2f(Space::ConvertWidth(pos.X + degree), Space::ConvertHeight(pos.Y - degree));
			glVertex2f(Space::ConvertWidth(pos.X + degree), Space::ConvertHeight(pos.Y + degree));
			glVertex2f(Space::ConvertWidth(pos.X - degree), Space::ConvertHeight(pos.Y + degree));
			glEnd();

			// Get rid of the flock's old pos.
			boid->CurPos = boid->NewPos;
		}
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void generateThreads()
{
	for (int i = 0; i < Flocks.size(); i++) {
		for (int x = 0; x < Flocks.at(i)->Boids.size(); x++) {
			new std::thread(boidThread, i, x);
		}
	}
}

void generateFlocks()
{
	int numFlocks = 100; // RNG::getNextInt() % (7 - 3) + 30;

	for (int i = 0; i < numFlocks; i++) {
		int numBirds = 20; // RNG::getNextInt() % (20 - 10) + 100;
		TotalBoids += numBirds;

		Flocks.push_back(new Flock());

		auto flock = Flocks.at(Flocks.size() - 1);
		flock->FocusPoint = Vector2f(RNG::getNextInt(0, WINDOW_WIDTH), RNG::getNextInt(0, WINDOW_HEIGHT));
		flock->R = RNG::getNextInt(0, 256);
		flock->G = RNG::getNextInt(0, 256);
		flock->B = RNG::getNextInt(0, 256);


		while (numBirds--) {
			auto boid = new Boid();
			boid->CurPos.X = RNG::getNextInt(0, WINDOW_WIDTH); // RNG::getNextInt() % WINDOW_WIDTH;
			boid->CurPos.Y = RNG::getNextInt(0, WINDOW_HEIGHT); // RNG::getNextInt() % WINDOW_HEIGHT;
			Flocks.at(i)->Boids.push_back(boid);
		}
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
	generateThreads();

	int tmrGraphics = 0;
	int tmrState = 0;
	int tmrFrameRate = GetTickCount64();
	int fps = 0;

	while (true) {
		auto tick = GetTickCount64();

		if (tick > tmrGraphics) {
			glutMainLoopEvent();
			fps++;
		}

		if (tick > tmrFrameRate) {
			std::string newTitle = "Assignment 1: Boids " + std::to_string(fps);
			glutSetWindowTitle(newTitle.c_str());
			tmrFrameRate = tick + 1000;
			fps = 0;
		}

		if (tick > tmrState) {
			Cond.lock();
			if (Cond.Count != TotalBoids) {
				Cond.unlock();
				std::this_thread::yield();
				continue;
			}

			Cond.State = !Cond.State;
			Cond.Count = 0;
			Cond.unlock();

			tmrState = tick + 33;
		}	
	}
}