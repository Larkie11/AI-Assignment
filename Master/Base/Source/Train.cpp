#include "Train.h"

Train::Train()
{
}

Train::~Train()
{
}

void Train::Update(double dt)
{
	timer++;
}

bool Train::checkTrainIsReady()
{
	if (commutersBoarded == 0 || commutersAlighted == 0)
	return true;
}

void Train::setCommutersBoarded(int x)
{
	this->commutersBoarded = x;
}

int Train::getCommutersBoarded()
{
	return commutersBoarded;
}

void Train::setCommutersAlighted(int x)
{
	this->commutersAlighted = x;
}

int Train::getCommutersAlighted()
{
	return commutersAlighted;
}


