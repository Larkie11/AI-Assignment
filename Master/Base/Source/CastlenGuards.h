#pragma once
#include "Vertex.h"
#include "Vector3.h"
#include "ChangeMesh.h"
#include "MyMath.h"

class CastlenGuards
{
public:
	CastlenGuards();
	virtual ~CastlenGuards();
	enum Castle
	{
		OPEN,
		CLOSE,
		DEFENCE,
	};
	enum Guard
	{
		MOVINGOUT,
		MOVINGIN,
		IDLING,
		ATTACKING,
		MOVINGL,
		MOVINGUP,
		MOVINGR,
		MOVINGD,
	};
	struct Guards
	{
		Vertex position;
		Vector3 scale;
		bool stopAnimation;
		ChangeMesh* guardMesh;
		Guard guardState;
	};
	void UpdateCastlenGuards(double dt);
	float RandomInt;
	int TempRandomInt;
	Castle castleState;
	Vertex doorPos, guard2Pos;
	Vector3 guard2Scale;
	bool guardMoveLeft, stopGuard2Anim;
	Guard guardState;
	Guards guard1, guard2;
	int close = 0;
	int open = 0;
	bool addedCount = false;
};