#pragma once
#include "ChangeMesh.h"
#include "Vertex.h"
#include <vector>

using std::vector;

class AppleSpawning
{
public:
	enum ApplesStates
	{
		SPAWNING,
		SPAWNED,
		ROTTING,
		DECAYED,
	};
	struct Apples
	{
		ApplesStates appleStates;
		ChangeMesh *appleMesh;
		Vertex position;
		float timer;
		bool spawned;
		Vertex newPosition;
		float despawn;
		int probability;
		int randomProb;
		float probabilityCountDown;
		bool notRotted = false;
	};

	vector<AppleSpawning::Apples> GetAppleVec();
	vector<Vertex>GetTreeVec();
	void Init();
	void InitApples();
	void InitTrees();
	void RespawnApples();
	void UpdateApplesFSM(double dt);
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