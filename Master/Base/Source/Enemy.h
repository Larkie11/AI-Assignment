#pragma once
#include "Vector3.h"

enum Behavior
{
	GOTOWP,
	IDLE,
	RUN,
};

class Enemy
{
public:
	Enemy();
	virtual ~Enemy();
	void Init(Behavior behavior, Vector3 moveto, Vector3 myposition, int idleprobability);
	void Update(double dt,int minX, int maxX, int minY, int maxY);
	Vector3 GetPosition();
	Vector3 GetWPPosition();
	Behavior GetState();
	void SetState(Behavior behavior);
private:
	Behavior behavior;
	Vector3 moveto;
	int idleprobability;
	Vector3 myposition;
	bool arrived;
	int randprob;
	float distance;
	float idletimer;
};