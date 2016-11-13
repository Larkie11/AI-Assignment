#ifndef SCENE_1_H
#define SCENE_1_H
#include "SceneBase.h"
#include "Camera3.h"
#include "Map.h"
#include "PlayerInfo.h"
#include "GameObject.h"
#include "CharacterData.h"
#include "ChangeMesh.h"

class Scene1 : public SceneBase
{
public:
	Scene1();
	~Scene1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();
	void RenderMap();
	void RenderGO();
	//Update Functions
	void PlayerUpdate(double dt); // Update to the player 
	void GOupdate(double dt); // Main GO Collisions
	void MapUpdate(double dt);
	int RandomInteger(int lowerLimit, int upperLimit);
	void CastleFSMUpdate(double dt);
	void SpawnAppleFSMUpdate(double dt);
	void HealPointFSMUpdate(double dt);
	void KingSlimeFSMUpdate(double dt);

	enum Castle
	{
		OPEN,
		CLOSE,
	};
	struct Guards
	{ 
		Vertex position;
		Vector3 scale;
		bool stopAnimation;
		ChangeMesh* guardMesh;
	};
	struct Apples
	{
		Vertex position;
		int timer;
		bool spawned;
		Vertex newPosition;
	};
	enum HealPoint
	{
		IDLE,
		HEAL,
		REST,
	};

	enum KingSlime
	{
		LAZE,
		MOVE,
		CHASE,
		RUN,
		EAT,
	};

private:
	int RandomInt;
	int TempRandomInt;
	bool playCastleAnim = false;
	CMap* m_cMap;	// Handle to the tilemaps
	Castle castleState;
	Vertex doorPos,guard2Pos;
	Vector3 guard2Scale;
	bool guardMoveLeft,stopGuard2Anim;

	ChangeMesh *guard2Mesh;
	Guards guard1, guard2;
	Vertex treePosition1, treePosition2;
	vector<Vertex>treePositions;
	vector<Apples>applePositions;
	Apples applePositions1;

	//Heal Point
	int PP;
	int PPcounter;
	HealPoint healpointState;
	Vertex healpointPos;

	//Kingslime
	int Hunger;
	int HungerCounter;
	int RandomInt2;
	int TempRandomInt2;
	int MoveCounter;
	int RandomMoveX;
	int RandomMoveY;
	KingSlime KSstate;
	Vertex KSpos;
	Vector3 vel;
	float p_speed;
};

#endif