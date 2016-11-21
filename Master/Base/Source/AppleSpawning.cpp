#include "AppleSpawning.h"
#include "MyMath.h"

AppleSpawning::AppleSpawning()
{

}
AppleSpawning::~AppleSpawning()
{

}
void AppleSpawning::Init()
{
	InitTrees();
	InitApples();
}
void AppleSpawning::InitTrees()
{
	for (int i = 1; i < 3; i++)
	{
		int randY = Math::RandIntMinMax(100, 200) * i;
		int randX = Math::RandIntMinMax(280, 300) * i;
		trees.pos.Set(randX, randY, 1);
		treeVec.push_back(trees);
	}
}
void AppleSpawning::InitApples()
{
	for (int i = 0; i < Math::RandIntMinMax(1, 5); i++)
	{
		int randTreePosition = Math::RandIntMinMax(-130, 10);
		int randTree = Math::RandIntMinMax(0, treeVec.size() - 1);
		apples.timer = Math::RandIntMinMax(0, 5);
		apples.spawned = false;
		apples.position.x = treeVec[randTree].pos.x - randTreePosition;
		apples.position.y = treeVec[randTree].pos.y + 90;
		apples.newPosition.Set(apples.position.x, apples.position.y - 80, 1);
		apples.appleStates = Apples::SPAWNING;
		apples.appleMesh = new ChangeMesh();
		apples.probability = 20;
		apples.randomProb = Math::RandIntMinMax(0, 100);
		apples.probabilityCountDown = 30;
		apples.despawn = 30;
		appleVec.push_back(apples);
	}
}
vector<Apples> AppleSpawning::GetAppleVec()
{
	return appleVec;
}
vector<Vertex> AppleSpawning::GetTreeVec()
{
	return treeVec;
}
void AppleSpawning::RespawnApples()
{
	for (int i = 0; i < appleVec.size(); i++)
	{
		if (appleVec[i].despawn <= 0)
		{
			appleVec[i].spawned = false;
			appleVec[i].randomProb = Math::RandIntMinMax(0, 100);
			appleVec[i].probabilityCountDown = 30;
			appleVec[i].appleStates = Apples::SPAWNING;
			int randTreePosition = Math::RandIntMinMax(-130, 10);
			int randTree = Math::RandIntMinMax(0, treeVec.size() - 1);
			appleVec[i].timer = Math::RandIntMinMax(0, 5);
			appleVec[i].position.x = treeVec[randTree].pos.x - randTreePosition;
			appleVec[i].position.y = treeVec[randTree].pos.y + 90;
			appleVec[i].newPosition.Set(appleVec[i].position.x, appleVec[i].position.y - 80, 1);
			appleVec[i].despawn = 30;
			appleVec[i].notRotted = false;
			addCount = false;
		}
	}
}
void AppleSpawning::UpdateApplesFSM(double dt)
{
	for (int i = 0; i < appleVec.size(); i++)
	{
		appleVec[i].timer -= dt;
		//cout << applePositions[i].timer << endl;
		//cout << i << " " << applePositions[i].newPosition.pos.y << endl;
		if (appleVec[i].timer <= 0 && !appleVec[i].spawned)
		{
			appleVec[i].spawned = true;
			/**/
		}
		if (appleVec[i].spawned && appleVec[i].despawn > 10)
		{
			appleVec[i].appleStates = Apples::SPAWNED;

			if (appleVec[i].position.y > appleVec[i].newPosition.y)
				appleVec[i].position.y--;

			appleVec[i].despawn -= dt;
			if (appleVec[i].randomProb < appleVec[i].probability)
			{
				if (addCount == false && appleVec[i].despawn <= 0)
				{
					countRot++;
					addCount = true;
				}
			}
			if (appleVec[i].randomProb > appleVec[i].probability)
			{
				if (addCount == false && appleVec[i].despawn <= 0)
				{
					countNoRot++;
					addCount = true;
				}
			}
		}
		if (appleVec[i].spawned && appleVec[i].despawn < 10)
		{
			if (appleVec[i].randomProb < appleVec[i].probability)
			{
				appleVec[i].appleStates = Apples::ROTTING;
			}
			else
			{
				appleVec[i].appleStates = Apples::DECAYED;
			}
			
		}
		if (appleVec[i].despawn <= 0)
		{
			RespawnApples();
		}
	}
}