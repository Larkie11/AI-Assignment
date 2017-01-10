#include "CastlenGuards.h"
#include <iostream>
#include "Enemy.h"

Enemy enemy;
CastlenGuards:: CastlenGuards()
{
	RandomInt = TempRandomInt = distance = openprob = open = close = defence = timer = 0;
	
	castleState = CLOSE;
	arrived = false;
	doorPos.SetZero();
	addedCount = false;
}
CastlenGuards::~CastlenGuards()
{
}
void CastlenGuards::InitCastlenGuards(int probabilitytoopen)
{
	RandomInt = Math::RandIntMinMax(0, 100);
	TempRandomInt = RandomInt;
	castleState = CLOSE;
	doorPos.Set(30, 300, 1);

	guard1.guardMesh = new ChangeMesh();
	guard1.scale.Set(30, 30, 1);
	guard1.GuardWaypointsOut.push_back(Vector3(145, 340, 1));
	guard1.GuardWaypointsOut.push_back(Vector3(135, 340, 1));
	guard1.GuardWaypointsOut.push_back(Vector3(132, 270, 1));
	guard1.GuardWaypointsOut.push_back(Vector3(43, 270, 1));
	guard1.GuardWaypointsOut.push_back(Vector3(40, 270, 1));

	guard1.wayPointID = 1;

	guard1.stopAnimation = false;
	guard1.guardState = Guards::IDLING;
	guard2.guardMesh = new ChangeMesh();
	guard2.scale.Set(30, 30, 1);

	guard2.GuardWaypointsOut.push_back(Vector3(155, 340, 1));
	guard2.GuardWaypointsOut.push_back(Vector3(150, 340, 1));
	guard2.GuardWaypointsOut.push_back(Vector3(150, 270, 1));
	guard2.GuardWaypointsOut.push_back(Vector3(243, 270, 1));
	guard2.GuardWaypointsOut.push_back(Vector3(245, 270, 1));

	guard2.wayPointID = 1;

	guard2.stopAnimation = false;
	guard2.guardState = Guards::IDLING;

	archer.position.Set(155, 200, 1);
	archer.archerwp.push_back(Vector3(155, 200, 1));
	archer.archerwp.push_back(Vector3(230, 200, 1));
	archer.wayPointID = 1;
	archer.guardMesh = new ChangeMesh();
	archer.scale.Set(30, 30, 1);
	archer.stopAnimation = true;

	guardList.push_back(guard1);
	guardList.push_back(guard2);
	openprob = probabilitytoopen;
}
float CastlenGuards::GetTimer()
{
	return timer;
}
vector<Guards> CastlenGuards::GetGuardList()
{
	return guardList;
}
Guards CastlenGuards::GetArcher()
{
	return archer;
}
Vector3 CastlenGuards::GetDoorPos()
{
	return doorPos;
}
Castle CastlenGuards::GetState()
{
	return castleState;
}
void CastlenGuards::SetState(Castle states)
{
	castleState = states;
}
int CastlenGuards::GetOpenProb()
{
	return openprob;
}
int CastlenGuards::GetOpenCounter()
{
	return open;
}
int CastlenGuards::GetCloseCounter()
{
	return close;
}
int CastlenGuards::GetDefenceCounter()
{
	return defence;
}
int CastlenGuards::GetRandomInt()
{
	return RandomInt;
}
int CastlenGuards::GetTempInt()
{
	return TempRandomInt;
}
void CastlenGuards::UpdateCastlenGuards(double dt, Vector3 enemyPosition)
{
	if (castleState != DEFENCE)
	{
		if (timer <= 0)
		{
			RandomInt = Math::RandIntMinMax(0, 100);
			TempRandomInt = RandomInt;
			addedCount = false;
			timer = Math::RandFloatMinMax(0, 30);
			if (TempRandomInt <= openprob)
			{
				castleState = OPEN;
				if (!addedCount)
				{
					open++;
					addedCount = true;
				}
			}
			else
			{
				castleState = CLOSE;
				if (!addedCount)
				{
					close++;
					addedCount = true;
				}
			}
		}


	}
	else if (castleState == DEFENCE && addedCount)
	{
		defence++;

		addedCount = false;
	}
	switch (castleState)
	{
	case OPEN:
		if (doorPos.y <= 350)
		{
			doorPos.y++;
		}
		for (int i = 0; i < guardList.size(); i++)
		{
			guardList[i].guardState = Guards::MOVINGD;

			if (!guardList[i].changePos && guardList[i].wayPointID < 3)
			{
				guardList[i].position = guardList[i].GuardWaypointsOut[0];
				guardList[i].changePos = true;
				guardList[i].stopAnimation = false;
				guardList[i].wayPointID = 1;
			}

			if (guardList[i].wayPointID >= 3 && guardList[i].wayPointID <= 5)
			{
				if (i == 0)
					guardList[i].guardState = Guards::MOVINGL;
				if (i == 1)
					guardList[i].guardState = Guards::MOVINGR;
			}

			if (guardList[i].wayPointID >= guardList[i].GuardWaypointsOut.size() - 1)
			{
				guardList[i].guardState = Guards::GUARDING;
				guardList[i].stopAnimation = true;
				timer -= dt;
			}
			guardList[i].nextPoint = guardList[i].GuardWaypointsOut[guardList[i].wayPointID];

			if (guardList[i].nextPoint != guardList[i].position)
			{
				direction = (guardList[i].nextPoint - guardList[i].position).Normalize();
			}
			else
			{
				guardList[i].stopAnimation = true;
				guardList[i].changePos = false;
			}

			distance = (guardList[i].nextPoint - guardList[i].position).LengthSquared();
			if (distance < 0.1 && guardList[i].wayPointID <= guardList[i].GuardWaypointsOut.size() - 1)
			{
				guardList[i].position = guardList[i].nextPoint;
				arrived = true;
			}
			else
				guardList[i].position = guardList[i].position + direction* 0.7;

			if (arrived && guardList[i].wayPointID < guardList[i].GuardWaypointsOut.size() - 1)
			{
				guardList[i].wayPointID++;
				arrived = false;
			}
		}
		break;
	case CLOSE:
		for (int i = 0; i < guardList.size(); i++)
		{
			if (i == 0)
				guardList[i].guardState = Guards::MOVINGR;
			if (i == 1)
				guardList[i].guardState = Guards::MOVINGL;
			if (!guardList[i].changePos && guardList[i].wayPointID > 0)
			{
				doorPos.Set(30, 350, 1);
				guardList[i].position = guardList[i].GuardWaypointsOut[3];
				guardList[i].changePos = true;
				guardList[0].wayPointID = 2;
				guardList[1].wayPointID = 2;

				guardList[i].stopAnimation = false;
			}
			if (guardList[i].wayPointID <= 0)
			{
				if (doorPos.y > 300)
				{
					doorPos.y--;
				}
				else
					timer -= dt;
				guardList[i].guardState = Guards::IDLING;
			}
			if (guardList[i].wayPointID == 1)
			{
				guardList[i].guardState = Guards::MOVINGUP;

			}
			guardList[i].nextPoint = guardList[i].GuardWaypointsOut[guardList[i].wayPointID];

			if (guardList[i].nextPoint != guardList[i].position)
			{
				direction = (guardList[i].nextPoint - guardList[i].position).Normalize();
			}
			else
			{
				//guardList[i].stopAnimation = true;
				guardList[i].changePos = false;
			}

			distance = (guardList[i].nextPoint - guardList[i].position).LengthSquared();
			if (distance < 0.1 && guardList[i].wayPointID >= 0)
			{
				guardList[i].position = guardList[i].nextPoint;
				arrived = true;
			}
			else
				guardList[i].position = guardList[i].position + direction* 0.7;

			if (arrived && guardList[i].wayPointID >= 0)
			{
				guardList[i].wayPointID--;
				arrived = false;

				if (guardList[i].wayPointID <= 0)
					guardList[i].wayPointID = 0;
			}
		}

		break;
	case DEFENCE:
		timer = 20;
		if (doorPos.y <= 350)
		{
			doorPos.y++;
		}
		archerout = true;

		archer.guardState = Guards::MOVINGOUT;

		if (archer.changePos &&archer.wayPointID < 3)
		{
			archer.position = archer.archerwp[0];
			archer.changePos = true;
			archer.stopAnimation = false;
			archer.wayPointID = 1;
		}

		if (archer.wayPointID >= archer.archerwp.size() - 1)
		{
			archer.guardState = Guards::ATTACKING;
			archer.stopAnimation = true;
			timer -= dt;
		}
		archer.nextPoint = archer.archerwp[archer.wayPointID];

		if (archer.nextPoint != archer.position)
		{
			direction = (archer.nextPoint - archer.position).Normalize();
		}
		else
		{
			archer.stopAnimation = true;
			archer.changePos = false;
		}

		distance = (archer.nextPoint - archer.position).LengthSquared();
		if (distance < 0.1 && archer.wayPointID <= archer.archerwp.size() - 1)
		{
			archer.position = archer.nextPoint;
			arrived = true;
		}
		else
			archer.position = archer.position + direction* 0.7;

		if (arrived && archer.wayPointID < archer.archerwp.size() - 1)
		{
			archer.wayPointID++;
			arrived = false;
		}

		for (int i = 0; i < guardList.size(); i++)
		{
			guardList[i].lastsavedposition = guardList[i].position;
			if (guardList[i].wayPointID == 1)
				guardList[i].wayPointID = 2;

			guardList[i].guardState = Guards::ATTACKING;
			distance = (enemyPosition - guardList[i].position).LengthSquared();
			{
				direction = (enemyPosition - guardList[i].position).Normalize();
				guardList[i].position = guardList[i].position + direction* Math::RandFloatMinMax(0.1, 0.5);
			}
			guardList[i].stopAnimation = false;

		}
		break;
	default:
		break;
	}
}