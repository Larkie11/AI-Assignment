#include "Bus.h"


Bus::Bus()
{
}

Bus::~Bus()
{
}

int Bus::GetCommuters()
{
	return currCommuters;
}

void Bus::SetCommuters(int x)
{
	this->currCommuters = x;
}

float Bus::GetSpeed()
{
	return speed;
}

void Bus::SetSpeed(float x)
{
	this->speed = x;
}