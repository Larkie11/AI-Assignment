#pragma once
#include "Vertex.h"
#include "Vector3.h"
#include "ChangeMesh.h"
#include "MyMath.h"
#include <vector>

using std::vector;
enum Castle
{
	OPEN,
	CLOSE,
	DEFENCE,
};
struct Guards
{
	typedef enum Guard
	{
		MOVINGOUT,
		MOVINGIN,
		IDLING,
		ATTACKING,
		MOVINGL,
		MOVINGUP,
		MOVINGR,
		MOVINGD,
		GUARDING,
	};
	Vector3 position;
	Vector3 scale;
	bool stopAnimation;
	ChangeMesh* guardMesh;
	Guard guardState;
	vector<Vector3>GuardWaypointsOut;
	vector<Vector3>GuardWaypointsIn;

	Vector3 nextPoint;
	int wayPointID;
	bool changePos = false;
};
class CastlenGuards
{
public:
	CastlenGuards();
	virtual ~CastlenGuards();
	
	
	Vector3 position1;
	vector<Guards> guardList;
	bool arrived = false;
	Vector3 direction;
	float distance = 0;
	void UpdateCastlenGuards(double dt);
	float RandomInt;
	int TempRandomInt;
	Castle castleState;
	Vertex doorPos, guard2Pos;
	Vector3 guard2Scale;
	bool guardMoveLeft, stopGuard2Anim;
	Guards guard1, guard2;
	int close = 0;
	int open = 0;
	bool addedCount = false;
};