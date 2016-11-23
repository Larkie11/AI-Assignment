#ifndef SCENE_1_H
#define SCENE_1_H
#include "SceneBase.h"
#include "Camera3.h"
#include "Map.h"
#include "PlayerInfo.h"
#include "GameObject.h"
#include "CharacterData.h"
#include "ChangeMesh.h"
#include "AppleSpawning.h"
#include "CastlenGuards.h"
#include "Enemy.h"

class AppleSpawning;
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
	void InitFSM();
	void RenderFSM();
	void RenderFSMText();
	void CastleFSMUpdate(double dt);
	void SpawnAppleFSMUpdate(double dt);
	void HealPointFSMUpdate(double dt);
	void KingSlimeFSMUpdate(double dt);
	void RenderCircle(float x, float y, float radius, float r, float g, float b);
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
	bool playCastleAnim = false;
	CMap* m_cMap;	// Handle to the tilemaps
	Vector3 castlePostion;
	Vector3 castleScale;
	Vector3 testPosition;
	AppleSpawning* apples;
	CastlenGuards* castlenguards;
	Enemy* enemy;
	int smallest = 0;
	bool addedCount = false;
	float distancetoenemy = 0;
	float distancetoenemy1 = 0;
	Vector3 directionenemy;
	Vector3 directionenemy1;

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