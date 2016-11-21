#include "CastlenGuards.h"
#include <iostream>

CastlenGuards:: CastlenGuards()
{
	RandomInt = Math::RandIntMinMax(0, 10);
	TempRandomInt = RandomInt;
	castleState = CLOSE;
	doorPos.pos.Set(30, 300, 1);

	guard1.guardMesh = new ChangeMesh();
	guard1.scale.Set(30, 30, 1);
	guard1.GuardWaypointsOut.push_back(Vector3(155, 340, 1));
	guard1.GuardWaypointsOut.push_back(Vector3(150, 340, 1));
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

	guardList.push_back(guard1);
	guardList.push_back(guard2);
}
CastlenGuards::~CastlenGuards()
{
}
void CastlenGuards::UpdateCastlenGuards(double dt)
{
	if (RandomInt <= 0)
	{
		RandomInt = Math::RandIntMinMax(0, 10);
		TempRandomInt = RandomInt;
		addedCount = false;
	}
	if (TempRandomInt % 2 == 0)
	{
		castleState = OPEN;
		if (!addedCount)
		{
			open++;
			addedCount = true;
		}
	}
	//Odd number castle is closed
	else
	{
		castleState = CLOSE;
		if (!addedCount)
		{
			close++;
			addedCount = true;
		}
	}
	switch (castleState)
	{
	case OPEN:
		if (doorPos.pos.y <= 350)
		{
			doorPos.pos.y++;
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
				guardList[i].guardState = Guards::IDLING;
				guardList[i].stopAnimation = true;
				RandomInt -= dt;
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
				std::cout << guardList[0].wayPointID << std::endl;
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
				doorPos.pos.Set(30, 350, 1);
				guardList[i].position = guardList[i].GuardWaypointsOut[3];
				guardList[i].changePos = true;
				guardList[0].wayPointID = 2;
				guardList[1].wayPointID = 2;

				guardList[i].stopAnimation = false;
			}
			if (guardList[i].wayPointID <= 0)
			{
				if (doorPos.pos.y > 300)
				{
					doorPos.pos.y--;
				}
				else
				RandomInt -= dt;
				guardList[i].guardState = Guards::IDLING;
			}
			if (guardList[i].wayPointID == 1)
			{
				guardList[i].guardState = Guards::MOVINGUP;
				
			}
			std::cout << guardList[i].wayPointID << std::endl;
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
	default:
		break;
	}
}
