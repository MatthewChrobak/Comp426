#include "Flock.h"
#include "tbb\parallel_for_each.h"

void Flock::updateBoids()
{

	//tbb::parallel_for(tbb::blocked_range<int>(0, this->Boids.size()), ->up
	tbb::parallel_for_each(
		this->Boids.begin(), 
		this->Boids.end(), 
		[&](Boid* b) { b->updateVelocity(this->Boids); }
	);
}