#pragma once
#include <mutex>

class BoidLock : public std::mutex
{
public:
	bool State = true;
	int Count = 0;
};

