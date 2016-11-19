#include "CastlenGuards.h"

CastlenGuards:: CastlenGuards()
{
	RandomInt = Math::RandIntMinMax(0, 10);
	TempRandomInt = RandomInt;
	castleState = CLOSE;
	doorPos.pos.Set(30, 300, 1);

	guard1.position.pos.Set(155, 320, 1);
	guard1.guardMesh = new ChangeMesh();
	guard1.scale.Set(0.1, 0.1, 1);
	guard1.stopAnimation = false;
	guard1.guardState = IDLING;

	guard2.position.pos.Set(162, 360, 1);
	guard2.guardMesh = new ChangeMesh();
	guard2.scale.Set(0.1, 0.1, 1);
	guard2.stopAnimation = false;
	guard2.guardState = IDLING;
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
		if (castleState == CLOSE)
		{
			//Rest guard positions
			if (guard1.position.pos.y != 320 || guard2.position.pos.x != 162)
			{
				guard1.position.pos.y = 320;
				guard2.position.pos.y = 360;
				guard2.position.pos.x = 162;
			}
		}
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
		if (guard2.scale.x <= 30)
		{
			guard2.scale.x++;
			guard2.position.pos.x -= 0.5;
			guard2.scale.y++;

			if (guard1.scale.x <= 30)
			{
				guard1.scale.x++;
				guard1.scale.y++;

				guard1.position.pos.x -= 0.5;
			}
		}
		if (guard1.position.pos.y >= 270)
		{
			guard1.position.pos.y -= 0.5;
			guard1.guardState = MOVINGD;
		}
		else
		{
			guard1.guardState = MOVINGL;
			if (guard1.position.pos.x >= 25)
				guard1.position.pos.x--;
			else
			{
				guard1.guardState = IDLING;
				guard1.stopAnimation = true;
			}
		}
		if (guard2.position.pos.y > 270)
		{
			guard2.position.pos.y -= 0.5;
			guard2.guardState = MOVINGD;
		}
		else
		{
			guard2.guardState = MOVINGR;
			if (guard2.position.pos.x <= 250)
				guard2.position.pos.x++;
			else
			{
				guard2.guardState = IDLING;
				guard2.stopAnimation = true;
				RandomInt -= dt;
			}
		}
		break;
	case CLOSE:
		if (guard1.position.pos.x <= 140)
		{
			guard1.position.pos.x++;
			guard1.guardState = MOVINGR;
			guard1.stopAnimation = false;
		}
		else if (guard1.position.pos.y <= 350)
		{
			guard1.position.pos.y++;
			guard1.guardState = MOVINGUP;
		}

		if (guard1.position.pos.x >= 80)
		{
			if (guard2.position.pos.x >= 145)
			{
				guard2.position.pos.x--;
				guard2.guardState = MOVINGL;
				guard2.stopAnimation = false;
			}
			else if (guard2.position.pos.y <= 340)
			{
				guard2.position.pos.y++;
				guard2.guardState = MOVINGUP;
			}
			else if (doorPos.pos.y >= 300)
			{
				doorPos.pos.y--;

				if (guard1.scale.x >= 0.01)
				{
					guard1.scale.x -= 0.5;
					guard1.scale.y -= 0.5;
					guard1.position.pos.x += 0.3;
				}
				if (guard2.scale.x >= 0.01)
				{
					guard2.scale.x -= 0.5;
					guard2.scale.y -= 0.5;
					guard2.position.pos.x += 0.3;
				}
			}
			else
			{
				RandomInt -= dt;
			}
		}
		break;
	default:
		break;
	}
}
