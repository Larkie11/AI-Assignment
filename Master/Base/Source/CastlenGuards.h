#pragma once
#include "Vertex.h"
#include "Vector3.h"
#include "ChangeMesh.h"
#include "MyMath.h"
#include "MessageBoard.h"
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
	vector<Vector3>archerwp;
	Vector3 lastsavedposition;

	Vector3 nextPoint;
	int wayPointID;
	bool changePos = false;
	bool archerPos = false;
	Vector3 archNextPoint;
	bool arrived = false;
	int health =50;
};
class CastlenGuards
{
public:
	CastlenGuards();
	virtual ~CastlenGuards();
	void SetState(Castle states);
	Castle GetState();
	vector<Guards>GetGuardList();
	Vector3 GetDoorPos();
	MessageBoard* GetMessageBoard();
	void SetHealth(int i, int whichguard);
	void MinusHealth(int i, int whichguard);
	void InitCastlenGuards(int probabilitytoopen);
	void UpdateCastlenGuards(double dt, Vector3 enemyPosition);
	int GetOpenCounter();
	int GetCloseCounter();
	Guards GetArcher();
	
	int GetOpenProb();
	int GetRandomInt();
	int GetTempInt();
	int GetDefenceCounter();
	float GetTimer();
private:
	Vector3 position1;
	vector<Guards> guardList;
	bool arrived = false;
	float timer;
	Vector3 direction;
	float distance;
	float RandomInt;
	int TempRandomInt;
	int openprob;
	Castle castleState;
	Vector3 doorPos;
	bool guardMoveLeft, stopGuard2Anim;
	Guards guard1, guard2, archer;
	bool archerout = false;
	bool archArrived = false;
	int close;
	int open;
	int defence;
	bool addedCount;
	MessageBoard* mb;
};