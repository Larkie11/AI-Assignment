#ifndef SCENE_1_H
#define SCENE_1_H
#include "SceneBase.h"
#include "Camera3.h"
#include "Map.h"
#include "PlayerInfo.h"
#include "GameObject.h"
#include "CharacterData.h"


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
	void FMSUpdate(double dt);
	enum Castle
	{
		OPEN,
		CLOSE,
	};
private:
	int RandomInt;
	int TempRandomInt;
	bool playCastleAnim = false;
	CMap* m_cMap;	// Handle to the tilemaps
	Castle castleState;
	Vertex doorPos;
};

#endif