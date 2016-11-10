#ifndef TRAIN_H
#define TRAIN_H

#include "GameObject.h"

class Train
{
public:

	enum TRAIN_STATES
	{
		S_MOVE,
		S_STOP,
		S_OPEN,
		S_CLOSE,
	};

	Train();
	~Train();

	virtual void Update(double dt);
	// checks if all the commuters hazzzzz boarded or all the commuters havvve alighted
	bool checkTrainIsReady();
	// getters and setters
	void setCommutersBoarded(int x);
	int getCommutersBoarded();
	void setCommutersAlighted(int x);
	int getCommutersAlighted();
	// probabilities


private:

	Vector3 position;
	float timer;
	int commutersBoarded;
	int commutersAlighted;
	const int totalCommuters = 100;

};



#endif