#ifndef BUS_H
#define BUS_H

#include "GameObject.h"
//#include "Traffic.h"

class Bus
{
public:

	enum BUS_STATES
	{
		S_FULL_SPEED,
		S_HALF_SPEED,
		S_STOP,
	};

	Bus();
	~Bus();

	virtual void Update(double dt);
	int GetCommuters();
	void SetCommuters(int x);
	float GetSpeed();
	void SetSpeed(float x);

private:

	Vector3 position;
	float speed;
	int currCommuters;
	const int totalCommuters = 50;

};



#endif