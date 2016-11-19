#include "Scene1.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "SpriteAnimation.h"

Scene1::Scene1()
: m_cMap(NULL)
{
}

Scene1::~Scene1()
{
	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}
}

void Scene1::Reset()
{
}

void Scene1::Init()
{
	SceneBase::Init();
	InitFSM();
	srand((unsigned)time(NULL));
	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(Application::GetInstance().GetScreenHeight(), Application::GetInstance().GetScreenWidth(), 32);
	m_cMap->LoadMap("Data//MapData_WM2.csv");
	apples = new AppleSpawning();
	apples->Init();
}

void Scene1::InitFSM()
{
	RandomInt = RandomInteger(30, 70);
	TempRandomInt = RandomInt;
	castleState = CLOSE;
	doorPos.pos.Set(30, 300, 1);

	guard1.guardMesh = new ChangeMesh();
	guard1.position.pos.Set(155, 320, 1);
	guard1.scale.Set(0.1, 0.1, 1);
	guard1.stopAnimation = false;

	guard2.guardMesh = new ChangeMesh();
	guard2.position.pos.Set(162, 360, 1);
	guard2.scale.Set(0.1, 0.1, 1);
	guard2.stopAnimation = false;
	guard2.guardMesh->SetNewMesh(meshList[GEO_GUARDS]);

	/*for (int i = 1; i < 3; i++)
	{
		int randY = RandomInteger(100, 200) * i;
		int randX = RandomInteger(280, 300) * i;
		treePosition1.pos.Set(randX, randY, 1);
		treePositions.push_back(treePosition1);
	}
	for (int i = 0; i < RandomInteger(1, 5); i++)
	{
		int randTreePosition = RandomInteger(-130, 10);
		int randTree = RandomInteger(0, treePositions.size() - 1);
		applePositions1.timer = RandomInteger(20, 50);
		applePositions1.spawned = false;
		applePositions1.position.pos.x = treePositions[randTree].pos.x - randTreePosition;
		applePositions1.position.pos.y = treePositions[randTree].pos.y + 90;
		applePositions1.newPosition.pos.Set(applePositions1.position.pos.x, applePositions1.position.pos.y - 80, 1);
		applePositions1.appleStates = SPAWNING;
		applePositions1.appleMesh = new ChangeMesh();
		applePositions1.probability = 50;
		applePositions1.randomProb = RandomInteger(0, 100);
		applePositions1.probabilityCountDown = applePositions1.randomProb;
		applePositions1.appleMesh->SetNewMesh(meshList[GEO_APPLES]);
		applePositions1.despawn = 500;
		applePositions.push_back(applePositions1);
	}*/

	//Heal Point data
	healpointState = HEAL;
	PP = 3;
	PPcounter = 100;
	healpointPos.pos.Set(450, 100, 1);

	//KingSlime data
	Hunger = 10;
	HungerCounter = 100;
	MoveCounter = 100;
	RandomInt2 = RandomInteger(1, 200);
	TempRandomInt2 = RandomInt2;
	RandomMoveX = RandomInteger(-5, 5);
	RandomMoveY = RandomInteger(-5, 5);
	KSstate = LAZE;
	KSpos.pos.Set(500, 300, 1);
}

void Scene1::PlayerUpdate(double dt)
{
}

void Scene1::GOupdate(double dt)
{
}

void Scene1::MapUpdate(double dt)
{
}

void Scene1::SpawnAppleFSMUpdate(double dt)
{
	//for (int i = 0; i < applePositions.size(); i++)
	//{
	//	applePositions[i].timer -= dt;
	//	//cout << applePositions[i].timer << endl;
	//	//cout << i << " " << applePositions[i].newPosition.pos.y << endl;
	//	if (applePositions[i].timer <= 0 && !applePositions[i].spawned)
	//	{
	//		applePositions[i].spawned = true;
	//		/**/
	//	}
	//	if (applePositions[i].spawned)
	//	{
	//		if (applePositions[i].position.pos.y > applePositions[i].newPosition.pos.y)
	//			applePositions[i].position.pos.y--;
	//		 
	//		if (applePositions[i].randomProb <= applePositions[i].probability)
	//		{
	//			applePositions[i].despawn -=  dt;
	//			//cout << i << " " << applePositions[i].despawn << endl;
	//		}
	//		else
	//		{
	//			applePositions[i].probabilityCountDown -= dt * 3;
	//			if (applePositions[i].probabilityCountDown <= 0)
	//			{
	//				applePositions[i].randomProb = RandomInteger(0, 100);
	//				applePositions[i].probabilityCountDown = applePositions[i].randomProb;
	//			}
	//		}
	//	}
	//	if (applePositions[i].spawned && applePositions[i].despawn < 250)
	//	{
	//		applePositions[i].appleMesh->SetNewMesh(meshList[GEO_ROTTENAPPLE]);
	//	}
	//	if (applePositions[i].despawn <= 0)
	//	{
	//		applePositions[i].spawned = false;
	//		applePositions[i].randomProb = RandomInteger(0, 100);
	//		applePositions[i].probabilityCountDown = applePositions[i].randomProb;
	//		applePositions[i].appleMesh->SetNewMesh(meshList[GEO_APPLES]);
	//	}
	//}
}

void Scene1::CastleFSMUpdate(double dt)
{
	//Update sprite animation
	SpriteAnimation *castle = dynamic_cast<SpriteAnimation*>(meshList[GEO_CASTLE]);
	if (castle)
	{
		castle->Update(dt);
		castle->m_anim->animActive = true;
	}
	SpriteAnimation *guards = dynamic_cast<SpriteAnimation*>(guard1.guardMesh->GetNewMesh());
	if (guards && !guard1.stopAnimation)
	{
			guards->Update(dt);
			guards->m_anim->animActive = true;
	}
	SpriteAnimation *guards2 = dynamic_cast<SpriteAnimation*>(guard2.guardMesh->GetNewMesh());
	if (guards2 && !guard2.stopAnimation)
	{
		guards2->Update(dt);
		guards2->m_anim->animActive = true;
	}
	//Random another number for next state
	if (RandomInt <= 0)
	{
		RandomInt = RandomInteger(300, 400);
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
	//If even number, castle is open
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
	case Scene1::OPEN:
		guardState = MOVINGOUT;
		if (doorPos.pos.y <= 350)
		{
			doorPos.pos.y++;
		}
		break;
	case Scene1::CLOSE:
		guardState = MOVINGIN;
		break;
	default:
		break;
	}
	switch (guardState)
	{
	case Scene1::MOVINGOUT:
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
			guard1.guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
		}
		else
		{
			guard1.guardMesh->SetNewMesh(meshList[GEO_GUARDSL]);
			if (guard1.position.pos.x >= 25)
				guard1.position.pos.x--;
			else
			{
				guard1.guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
				guard1.stopAnimation = true;
			}
		}
		if (guard2.position.pos.y >= 270)
		{
			guard2.position.pos.y -= 0.5;
			guard2.guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
		}
		else
		{
			guard2.guardMesh->SetNewMesh(meshList[GEO_GUARDSR]);
			if (guard2.position.pos.x <= 250)
				guard2.position.pos.x++;
			else
			{
				guard2.guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
				guard2.stopAnimation = true;
				RandomInt -= dt;
			}
		}
		break;
	case Scene1::MOVINGIN:
		if (guard1.position.pos.x <= 140)
		{
			guard1.position.pos.x++;
			guard1.guardMesh->SetNewMesh(meshList[GEO_GUARDSR]);
			guard1.stopAnimation = false;
		}
		else if (guard1.position.pos.y <= 350)
		{
			guard1.position.pos.y++;
			guard1.guardMesh->SetNewMesh(meshList[GEO_GUARDSUP]);
		}

		if (guard1.position.pos.x >= 80)
		{
			if (guard2.position.pos.x >= 145)
			{
				guard2.position.pos.x--;
				guard2.guardMesh->SetNewMesh(meshList[GEO_GUARDSL]);
				guard2.stopAnimation = false;
			}
			else if (guard2.position.pos.y <= 340)
			{
				guard2.position.pos.y++;
				guard2.guardMesh->SetNewMesh(meshList[GEO_GUARDSUP]);
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
	case Scene1::IDLING:
		break;
	default:
		break;
	}
}

void Scene1::HealPointFSMUpdate(double dt)
{
	//HealPoint FSM
	if (healpointState == IDLE)
	{

	}
	else if (healpointState == HEAL)
	{
		PPcounter-- * dt;
		if (PPcounter == 0)
		{
			PP -= 1;
			PPcounter = 100;
			if (PP <= 0)
				healpointState = REST;
		}
	}
	else if (healpointState == REST)
	{
		PPcounter-- * dt;
		if (PPcounter == 10)
		{
			PP += 1;
			PPcounter = 100;
			if (PP >= 10)
				healpointState = IDLE;
		}
	}
}

void Scene1::KingSlimeFSMUpdate(double dt)
{
	//King Slime FSM
	SpriteAnimation *KSidle = dynamic_cast<SpriteAnimation*>(meshList[GEO_KSIDLE]);
	if (KSidle)
	{
		KSidle->Update(dt);
		KSidle->m_anim->animActive = true;
	}
	SpriteAnimation *KSmoveL = dynamic_cast<SpriteAnimation*>(meshList[GEO_KSMOVEL]);
	if (KSmoveL)
	{
		KSmoveL->Update(dt);
		KSmoveL->m_anim->animActive = true;
	}
	SpriteAnimation *KSmoveR = dynamic_cast<SpriteAnimation*>(meshList[GEO_KSMOVER]);
	if (KSmoveR)
	{
		KSmoveR->Update(dt);
		KSmoveR->m_anim->animActive = true;
	}

	RandomInt2 -= dt* 0.001;
	if (RandomInt2 <= 0)
	{
		RandomInt2 = RandomInteger(1, 200);
		TempRandomInt2 = RandomInt2;
	}

	MoveCounter -= dt * 0.001;
	if (KSstate != EAT && MoveCounter <= 0)
	{
		MoveCounter = 300;
		Hunger--;
		if (Hunger == 0)
			KSstate = EAT;
		RandomMoveX = RandomInteger(-50, 50);
		RandomMoveY = RandomInteger(-50, 50);
	}

	if (KSstate != EAT && TempRandomInt2 % 2 == 0)
		KSstate = LAZE;
	else if (Hunger == 0)
		KSstate = EAT;
	else
	{
		KSstate = MOVE;
	}

	if (KSstate == MOVE)
	{
		if (KSpos.pos.x >= 0 && KSpos.pos.x <= 700)
		{
			KSpos.pos.x += RandomMoveX * dt;
		}
		if (KSpos.pos.y >= 0 && KSpos.pos.y <= 300)
		{
			KSpos.pos.y += RandomMoveY * dt;
		}
	}
	else if (KSstate == EAT)
	{
		Vector3 d;

		for (int i = 0; i < applePositions.size(); i++)
		{
			d.x = applePositions[i].position.pos.x - KSpos.pos.x;
			d.y = applePositions[i].position.pos.y - KSpos.pos.y;

			if (d.IsZero())
			{
				return;
			}
			p_speed = 10.f;
			this->vel = d.Normalized() * p_speed;
			KSpos.pos.x += vel.x * dt;
			KSpos.pos.y += vel.y * dt;

			float distanceToCheck = 0.04f * p_speed;
			if (d.LengthSquared() <= distanceToCheck * distanceToCheck)
			{
				applePositions[i].spawned = false;
				Hunger = 100;
			}
		}
	}
}

void Scene1::Update(double dt)
{
	SceneBase::Update(dt);
	apples->UpdateApplesFSM(dt);
	CastleFSMUpdate(dt);
	HealPointFSMUpdate(dt);
	KingSlimeFSMUpdate(dt);

	fps = (float)(1.f / dt);
}

int Scene1::RandomInteger(int lowerLimit, int upperLimit)
{
	return rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
}

void Scene1::RenderFSM()
{
	Render2DMeshWScale(guard1.guardMesh->GetNewMesh(), false, guard1.scale.x, guard1.scale.y, guard1.position.pos.x, guard1.position.pos.y, false);
	Render2DMeshWScale(guard2.guardMesh->GetNewMesh(), false, guard2.scale.x, guard2.scale.y, guard2.position.pos.x, guard2.position.pos.y, false);

	Render2DMeshWScale(meshList[GEO_DOOR], false, 250, 250, doorPos.pos.x, doorPos.pos.y, false);

	Render2DMeshWScale(meshList[GEO_CASTLE], false, 250, 250, 30, 300, false);
	/*for (int i = 0; i < applePositions.size(); i++)
	{
		if (applePositions[i].spawned)
		{
			Render2DMeshWScale(applePositions[i].appleMesh->GetNewMesh(), false, 30, 30, applePositions[i].position.pos.x, applePositions[i].position.pos.y, false);
		}
	}*/
	for (int i = 0; i < apples->GetAppleVec().size(); i++)
	{
		switch (apples->GetAppleVec()[i].appleStates)
		{
		case AppleSpawning::SPAWNING:
			apples->GetAppleVec()[i].appleMesh->SetNewMesh(meshList[GEO_APPLES]);
			break;
		case AppleSpawning::SPAWNED:
			apples->GetAppleVec()[i].appleMesh->SetNewMesh(meshList[GEO_APPLES]);
			break;
		case AppleSpawning::ROTTING:
			apples->GetAppleVec()[i].appleMesh->SetNewMesh(meshList[GEO_ROTTENAPPLE]);
			break;
		case AppleSpawning::DECAYED:
			apples->GetAppleVec()[i].appleMesh->SetNewMesh(meshList[GEO_DECAYAPPLE]);
			break;
		}
		if (apples->GetAppleVec()[i].spawned)
		{
			Render2DMeshWScale(apples->GetAppleVec()[i].appleMesh->GetNewMesh(), false, 30, 30, apples->GetAppleVec()[i].position.pos.x, apples->GetAppleVec()[i].position.pos.y, false);
		}
		cout << i << " " << apples->GetAppleVec()[i].randomProb << "   " << apples->GetAppleVec()[i].probability << endl;
	}
	for (int i = 0; i < apples->GetTreeVec().size(); i++)
	{
		Render2DMeshWScale(meshList[GEO_TREE], false, 170, 170, apples->GetTreeVec()[i].pos.x, apples->GetTreeVec()[i].pos.y, false);
	}

	//Render HealPoint
	if (healpointState == HEAL)
		Render2DMeshWScale(meshList[GEO_HEAL_HEAL], false, 10, 10, healpointPos.pos.x, healpointPos.pos.y, false);
	else if (healpointState == REST)
		Render2DMeshWScale(meshList[GEO_HEAL_REST], false, 10, 10, healpointPos.pos.x, healpointPos.pos.y, false);
	else
		Render2DMeshWScale(meshList[GEO_HEAL_IDLE], false, 10, 10, healpointPos.pos.x, healpointPos.pos.y, false);

	//Render Kingslime
	if (KSstate == IDLE)
		Render2DMeshWScale(meshList[GEO_KSIDLE], false, 80, 50, KSpos.pos.x, KSpos.pos.y, false);
	else if (KSstate == MOVE || KSstate == EAT)
	{
		if (RandomMoveX < 0)
			Render2DMeshWScale(meshList[GEO_KSMOVEL], false, 100, 80, KSpos.pos.x, KSpos.pos.y, false);
		else
			Render2DMeshWScale(meshList[GEO_KSMOVER], false, 100, 80, KSpos.pos.x, KSpos.pos.y, false);
	}
}

void Scene1::RenderFSMText()
{
	std::ostringstream ss;

	/*ss.str("");
	ss << "Spawning " << applePositions.size() << " apples";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 1), 20, 450, 3);
	*/
	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 30, 0, 0);
	ss.str("");
	ss << "Open: " << open << " Close: " << close;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, 20, 520);

	switch (castleState)
	{
	case OPEN:
		ss.str("");
		ss << "OPEN";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 20, 540);
		break;
	case CLOSE:
		ss.str("");
		ss << "CLOSE";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 20, 540);
		break;
	}
	ss.str("");
	ss << "To next random state " << RandomInt;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, 20, 570);
	ss.str("");
	ss << "Previous random state number " << TempRandomInt;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, 20, 585);

	ss.str("");
	ss << "KS Hunger: " << Hunger;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 500, 200);

	switch (healpointState)
	{
	case REST:
		ss.str("");
		ss << "RECOVERING ITSELF";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 180, 70);
		break;
	case IDLE:
		ss.str("");
		ss << "IDLING";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 180, 70);
		break;
	case HEAL:
		ss.str("");
		ss << "HEALING PLAYER";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 180, 70);
		break;
	}
	switch (KSstate)
	{
	case Scene1::LAZE:
		ss.str("");
		ss << "KS State:IDLING";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 500, 230);
		break;
	case Scene1::MOVE:
		ss.str("");
		ss << "KS State:MOVING";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 500, 230);
		break;
	case Scene1::CHASE:
		ss.str("");
		ss << "KS State:CHASING";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 500, 230);
		break;
	case Scene1::RUN:
		ss.str("");
		ss << "KS State:RUNNINIG";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 500, 230);
		break;
	case Scene1::EAT:
		ss.str("");
		ss << "KS State:EATING";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 500, 230);
		break;
	default:
		break;
	}
	ss.str("");
	ss << "PP: " << PP;
	ostringstream oss;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 180, 50);

	int y = 0;
	int x = 300;
	ostringstream oss2;
	for (int i = 0; i < apples->GetAppleVec().size(); i++)
	{
		x += 20;
		ss.str("");

		if (apples->GetAppleVec()[i].timer > 0)
			ss << "Apple " << i << " " << apples->GetAppleVec()[i].timer;

		if (apples->GetAppleVec()[i].spawned && apples->GetAppleVec()[i].despawn> 10)
			ss << "Apple " << i << " Spawned";

		if (apples->GetAppleVec()[i].appleStates == AppleSpawning::ROTTING)
			ss << "Apple " << i << " Rotting";

		if (apples->GetAppleVec()[i].appleStates == AppleSpawning::DECAYED)
			ss << "Apple " << i << " Decaying";
		
		oss2.str("");
		oss2 << "Rot Probability " << apples->GetAppleVec()[i].probability;

		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 1), 20, 100, x);
		RenderTextOnScreen(meshList[GEO_TEXT], oss2.str(), Color(0, 0, 1), 20, 400, x);
	}
	ss.str("");
	ss << apples->countNoRot << " " << apples->countRot;

	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 1), 20, 20, 300);
}

void Scene1::RenderMap()
{
	static float xpos = 0.f;
	static float ypos = 0.f;

	if (Application::IsKeyPressed('U'))
	{
		ypos += 0.5f;
	}
	if (Application::IsKeyPressed('J'))
	{
		ypos -= 0.5f;
	}
	if (Application::IsKeyPressed('H'))
	{
		xpos -= 0.5f;
	}
	if (Application::IsKeyPressed('K'))
	{
		xpos += 0.5f;
	}
	

	//RenderBackground(meshList[GEO_BACKGROUND]);
	RenderTileMap(meshList[GEO_TILESET1], m_cMap);
	
}

void Scene1::RenderGO()
{
}

void Scene1::Render()
{
	SceneBase::Render();
	RenderMap();
	RenderFSM();
	RenderFSMText();
}

void Scene1::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	delete apples;
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}