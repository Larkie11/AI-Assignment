#pragma once
#include "Vector3.h"

class Bullet
{
public:
	Bullet();
	~Bullet();
	void Update(double dt);
	void SetPosition(Vector3 pos,Vector3 dist);
	void SetTarget(Vector3 pos);
	Vector3 GetPosition();
	Vector3 gravity;
	Vector3 pos;
	Vector3 vel;
	Vector3 target;
	
private:
	float positiontill;
};