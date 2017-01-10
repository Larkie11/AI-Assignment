#pragma once
#include "ChangeMesh.h"
#include "Vertex.h"
#include <vector>

using std::vector;



struct Apples
{
	typedef enum ApplesStates
	{
		SPAWNING,
		SPAWNED,
		ROTTING,
		DECAYED,
	}; 
	ApplesStates appleStates;

	ChangeMesh *appleMesh;
	Vector3 position;
	float timer;
	bool spawned;
	Vector3 newPosition;
	float despawn;
	int probability;
	int randomProb;
	float probabilityCountDown;
	bool notRotted = false;
};

class AppleSpawning
{
public:
	vector<Apples> GetAppleVec();
	vector<Vertex>GetTreeVec();
	void Init(int probability);
	void InitApples(int probability);
	void InitTrees();
	void RespawnApples();
	void GetAppleState();
	void UpdateApplesFSM(double dt);
	void SetAppleDespawn(int despawn, int appleid);
	AppleSpawning();
	virtual ~AppleSpawning();
	int countRot = 0;
	int countNoRot = 0;

private:
	vector<Apples>appleVec;
	vector<Vertex>treeVec;
	Vertex trees;
	Apples apples;
	bool addCount = false;
	
};
