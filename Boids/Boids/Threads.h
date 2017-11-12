#pragma once
#include "Rules.h"

void boidThread(int flockIndex, int boidIndex)
{
	auto flock = Flocks.at(flockIndex);
	auto boid = flock->Boids.at(boidIndex);

	while (true) {

		while (!boid->RequiresLogicUpdate) {
			std::this_thread::yield();
		}
		auto rule1 = cohesionRule(flockIndex, boidIndex);
		auto rule2 = separationRule(flockIndex, boidIndex);
		auto rule3 = alignmentRule(flockIndex, boidIndex);
		auto rulePos = restrictPos(flockIndex, boidIndex);

		Vector newVelocity = boid->Velocity + rule1 + rule2 + rule3 + rulePos;

		boid->Velocity = newVelocity;
		restrictVelocity(flockIndex, boidIndex);

		boid->CurPos = boid->CurPos + boid->Velocity;
		boid->RequiresLogicUpdate = false;
		boid->HasUpdatedLogic = true;
	}
}

void flockThread(int flockIndex)
{
	auto flock = Flocks.at(flockIndex);

	for (int i = 0; i < flock->Boids.size(); i++) {
		new std::thread(boidThread, flockIndex, i);
	}

	while (true) {
		// Wait until we can go.
		while (!flock->RequiresLogicUpdate) {
			std::this_thread::yield();
		}


		// Flag them to go.
		for (int i = 0; i < flock->Boids.size(); i++) {
			auto boid = flock->Boids.at(i);
			boid->RequiresLogicUpdate = true;
		}
		flock->RequiresLogicUpdate = false;


		// Wait to update them.
		int i = 0;
		do {
			auto boid = flock->Boids.at(i);
			while (!boid->HasUpdatedLogic) {
				std::this_thread::yield();
			}
			boid->HasUpdatedLogic = false;
		} while (++i < flock->Boids.size());
	}
}