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
#include "Bullet.h"

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
	void UpdateFSM(double dt);
	void RenderFSM();
	void RenderFSMText();
	void CastleFSMUpdate(double dt);
	void SpawnAppleFSMUpdate(double dt);
	void HealPointFSMUpdate(double dt);
	void KingSlimeFSMUpdate(double dt);
	bool Detect(Vector3 pos1, Vector3 pos2, float radius);
	bool HPDetect(Vector3 pos1, Vector3 pos2, float radius);
	void RenderCircle(float x, float y, float radius, float r, float g, float b);
	MessageBoard* GetMessageBoard();
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
		ATTACK,
		RUN,
		EAT,
	};

	MessageBoard *mess;
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
	Vector3 directionenemy = Vector3(1,1,1);
	Vector3 directionenemy1 = Vector3(1, 1, 1);

	Bullet* shoot;
	float cooldown = 3.f;
	bool shot = false;
	//Heal Point
	int PP;
	int PPcounter;
	bool HPdetect;
	const float HPRadius = 60.f;
	float hpradiussize = HPRadius;
	HealPoint healpointState;
	Vertex healpointPos;


	float ShootingBullet = 5.f;
	//Kingslime
	int Hunger;
	int HungerCounter;
	int RandomInt2;
	int TempRandomInt2;
	int MoveCounter;
	int RandomMoveX;
	int RandomMoveY;
	int TempRandomIntX;
	int TempRandomIntY;
	const float KSRadius = 60.f;
	float radiussize = KSRadius;
	bool detect;
	int KSHP;
	int KSHPcounter;
	KingSlime KSstate;
	Vector3 KSpos;
	Vector3 vel;
	float p_speed;
	bool detected;
	bool isattacked;
	bool ishurted;
};

#endif