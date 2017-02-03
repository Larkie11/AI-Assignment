#include "CastlenGuards.h"
#include <iostream>
#include "Enemy.h"
#include <sstream>

Enemy enemy;

CastlenGuards:: CastlenGuards()
{
	RandomInt = TempRandomInt = distance = openprob = open = close = defence = timer = 0;
	castleState = CLOSE;
	arrived = false;
	doorPos.SetZero();
	addedCount = false;
	mb = new MessageBoard();
}
CastlenGuards::~CastlenGuards()
{
}
MessageBoard* CastlenGuards::GetMessageBoard()
{
	return mb;
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

	archer.position.Set(170, 350, 1);
	archer.archerwp.push_back(Vector3(170, 350, 1));
	archer.archerwp.push_back(Vector3(180, 350, 1));
	archer.archerwp.push_back(Vector3(230, 350, 1));
	archer.wayPointID = 1;
	archer.guardMesh = new ChangeMesh();
	archer.guardState = Guards::IDLING;
	archer.scale.Set(30, 30, 1);
	archer.stopAnimation = false;

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
void CastlenGuards::SetHealth(int i, int whichguard)
{
	guardList[whichguard].health = i;
}
void CastlenGuards::MinusHealth(int i, int whichguard)
{
	if (guardList[whichguard].health > 0)
	guardList[whichguard].health -= i;
}
void CastlenGuards::UpdateCastlenGuards(double dt, Vector3 enemyPosition, int& KSHP)
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
	if (KSHP <= 0)
	{
		mb->SetFromLabel("Guard");
		mb->SetToLabel("Archer");
		std::ostringstream oss;
		oss << "Archer, thanks for your help";
		mb->SetMessage(oss.str());
		archer.stopAnimation = false;
		archerout = false;
	}
	if (mb->GetMsg() == ("Archer, thanks for your help"))
	{
		archer.guardState = Guards::MOVINGIN;

		if (archer.wayPointID > 0)
		{
			archer.changePos = true;
			archer.position = archer.archerwp[2];
			archer.wayPointID = 0;
		}
	
		archer.nextPoint = archer.archerwp[archer.wayPointID];

		Vector3 direction2;
		if (archer.nextPoint != archer.position)
		{
			direction2 = (archer.nextPoint - archer.position).Normalize();
		}
		else
		{
			archer.changePos = false;
		}
		float distance2 = (archer.nextPoint - archer.position).LengthSquared();

		if (distance2 < 0.1)
		{
			archer.position = archer.nextPoint;
		}
		else
			archer.position = archer.position + direction2* 0.7;

		if (distance2 < 0.1 && archer.wayPointID > 0)
		{
			archer.wayPointID--;
			archer.arrived = false;
		}
		if (distance2 <= 0)
		{
			archer.wayPointID = 0;
			mb->SetMessage("");
			archer.guardState = Guards::IDLING;
			archer.stopAnimation = true;
			mb->SetFromLabel("");
			mb->SetToLabel("");
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
		if (mb->GetMsg() == "Archer to station please")
		{
			archerout = true;
		}

		if (archerout)
		{
			archer.guardState = Guards::MOVINGOUT;

			if (archer.changePos && archer.wayPointID < archer.archerwp.size() - 1)
			{
				archer.position = archer.archerwp[0];
				archer.changePos = true;
				archer.stopAnimation = false;
				archer.wayPointID = 1;
			}

			if (archer.wayPointID >= archer.archerwp.size())
			{
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
				archer.guardState = Guards::ATTACKING;

				for (int i = 0; i < guardList.size(); i++)
				{
					if (guardList[i].health <= 0)
					{
						mb->SetFromLabel("Archer");
						mb->SetToLabel("Guard"); 
						std::ostringstream oss;
						oss << "Replacement for guard " << i << " please";
						mb->SetMessage(oss.str());
					}
					if (guardList[0].health >0 && guardList[1].health > 0)
					{
						//mb->Reset();
					}
				}

			}

			distance = (archer.nextPoint - archer.position).LengthSquared();
			if (distance < 0.1 && archer.wayPointID <= archer.archerwp.size() - 1)
			{
				archer.position = archer.nextPoint;
				archer.arrived = true;
			}
			else
				archer.position = archer.position + direction* 0.7;

			if (archer.arrived && archer.wayPointID < archer.archerwp.size() - 1)
			{
				archer.wayPointID++;
				archer.arrived = false;
			}
		}
		if (doorPos.y <= 350)
		{
			doorPos.y++;
		}
		for (int i = 0; i < guardList.size(); i++)
		{
			if (guardList[0].health <= 0 && mb->GetMsg() == "Replacement for guard 0 please")
			{
				guardList[0].health = 100;
				guardList[0].position = guardList[i].GuardWaypointsOut[0];
				guardList[0].wayPointID = 1;
			}
			if (guardList[1].health <= 0 && mb->GetMsg() == "Replacement for guard 1 please")
			{
				guardList[1].health = 100;
				guardList[1].position = guardList[i].GuardWaypointsOut[0];
				guardList[1].wayPointID = 1;
			}
			//std::cout << guardList[i].health << std::endl;
			if (!guardList[i].changePos && guardList[i].wayPointID < 3)
			{
				guardList[i].position = guardList[i].GuardWaypointsOut[0];
				guardList[i].changePos = true;
				guardList[i].stopAnimation = false;
				guardList[i].wayPointID = 1;
			}

			if (guardList[i].wayPointID >= 3)
			{
				guardList[i].guardState = Guards::ATTACKING;
			}
			else
			{
				guardList[i].guardState = Guards::MOVINGD;
			}
			if (guardList[i].guardState == Guards::ATTACKING)
			{
				if (archer.guardState != Guards::ATTACKING && KSHP > 0)
				{
					mb->SetFromLabel("Guards");
					mb->SetToLabel("Archer");
					mb->SetMessage("Archer to station please");
				}
				guardList[i].lastsavedposition = guardList[i].position;
				if (guardList[i].wayPointID == 1)
					guardList[i].wayPointID = 2;

				distance = (enemyPosition - guardList[0].position).LengthSquared();
				Vector3 enemyPositionOffset = Vector3(enemyPosition.x, enemyPosition.y + 0.3, enemyPosition.z);
				float distance2 = (enemyPositionOffset - guardList[1].position).LengthSquared();
				{
					if (guardList[0].position.x <= enemyPosition.x)
						guardList[0].guardState = Guards::MOVINGR;
					else
						guardList[0].guardState = Guards::MOVINGL;

					if (guardList[1].position.x <= enemyPositionOffset.x)
						guardList[1].guardState = Guards::MOVINGR;
					else
						guardList[1].guardState = Guards::MOVINGL;

					
					direction = (enemyPosition - guardList[0].position).Normalize();
					Vector3 direction2 = (enemyPositionOffset - guardList[1].position).Normalize();

					if (distance > 400)
						guardList[0].position = (guardList[0].position + direction* Math::RandFloatMinMax(0.7, 1.5));

					if (distance < 500)
						guardList[0].guardState = Guards::ATTACKING;

					if (distance2 > 400)
						guardList[1].position = (guardList[1].position + direction2* Math::RandFloatMinMax(1, 1.5));

					if (distance2 < 500)
						guardList[1].guardState = Guards::ATTACKING;

					if (guardList[i].guardState == Guards::ATTACKING && KSHP > 0)
					{
						attackCounter -= dt;
						attacking = true;
					}
					if (attacking && attackCounter <= 0.f)
					{
						KSHP -= 1;
						attacking = false;
						attackCounter = 3.f;
					}
				}
				guardList[i].stopAnimation = false;
			}
			else
			{
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
				if (distance < 0.1 && guardList[i].wayPointID <= guardList[i].GuardWaypointsOut.size() < 3)
				{
					guardList[i].position = guardList[i].nextPoint;
					arrived = true;
				}
				else
					guardList[i].position = guardList[i].position + direction* 0.7;

				if (arrived && guardList[i].wayPointID < guardList[i].GuardWaypointsOut.size() < 3)
				{
					guardList[i].wayPointID++;
					arrived = false;
				}
			}
		}
		/*for (int i = 0; i < guardList.size(); i++)
		{
			

		}*/
		break;
	default:
		break;
	}
}