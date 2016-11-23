#include "Enemy.h"
#include "MyMath.h"
#include <iostream>

Enemy::Enemy()
{
	moveto.SetZero();
	behavior = GOTOWP;
	idleprobability = 0;
	myposition.SetZero();
	arrived = false;
	randprob = 0;
	distance = 100;
	idletimer = 20;
}
Enemy::~Enemy()
{

}
void Enemy::Init(Behavior behavior, Vector3 moveto, Vector3 myposition, int idleprobability)
{
	this->behavior = behavior;
	this->moveto = moveto;
	this->idleprobability = idleprobability;
	this->moveto = Vector3(Math::RandIntMinMax(0, 200), Math::RandIntMinMax(200, 290), 1);
	this->myposition = myposition;
}
Vector3 Enemy::GetPosition()
{
	return myposition;
}
Vector3 Enemy::GetWPPosition()
{
	return moveto;
}
Behavior Enemy::GetState()
{
	return behavior;
}
void Enemy::SetState(Behavior behavior)
{
	this->behavior = behavior;
}
void Enemy::SetPosition(Vector3 pos)
{
	myposition = pos;
}
void Enemy::Update(double dt,int minX, int maxX, int minY, int maxY)
{
	if (arrived)
		{
			if (behavior == GOTOWP)
			{
				randprob = Math::RandIntMinMax(0, 100);
			}
			moveto = Vector3(Math::RandIntMinMax(minX, maxX), Math::RandIntMinMax(minY, maxY), 1);

			if (randprob > idleprobability)
			{
				behavior = GOTOWP;
			}
			else
			{
				behavior = IDLE;
			}
			arrived = false;
		}
	if (behavior == GOTOWP)
	{
		Vector3 direction = (moveto - myposition).Normalize();
		distance = (myposition - moveto).LengthSquared();
		myposition = myposition + direction* 0.7;
	}
	if (behavior == IDLE)
	{
		idletimer -= dt;
	}
	if (behavior == RUN)
	{
		idletimer = 0;
	}
	if (idletimer <= 0)
	{
		idletimer = Math::RandFloatMinMax(3, 10);
		randprob = Math::RandIntMinMax(0, 100);
		arrived = true;
	}
	if (distance < 0.2)
	{
		arrived = true;
	}
	std::cout << "TIME " << idletimer << std::endl;
}