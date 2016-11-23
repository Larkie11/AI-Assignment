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
	castlenguards = new CastlenGuards();
	enemy = new Enemy();
	apples->Init();
	enemy->Init(Behavior::GOTOWP, Vector3(100, 100, 1), Vector3(100, 200, 1), 30);
	castlePostion.Set(30, 300, 1);
	testPosition.Set(30, 300, 1);
	castleScale.Set(250, 250, 1);
	castlenguards->InitCastlenGuards(30);
	directionenemy.SetZero();
	directionenemy1.SetZero();
	testPosition = enemy->GetPosition();
}

void Scene1::InitFSM()
{
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
	SpriteAnimation *villager = dynamic_cast<SpriteAnimation*>(meshList[GEO_VILLAGER]);
	if (villager)
	{
		villager->Update(dt);
		villager->m_anim->animActive = true;
	}
	SpriteAnimation *guards = dynamic_cast<SpriteAnimation*>(castlenguards->GetGuardList()[0].guardMesh->GetNewMesh());
	if (guards && !castlenguards->GetGuardList()[0].stopAnimation)
	{
			guards->Update(dt);
			guards->m_anim->animActive = true;
	}
	SpriteAnimation *guards2 = dynamic_cast<SpriteAnimation*>(castlenguards->GetGuardList()[1].guardMesh->GetNewMesh());
	if (guards2 && !castlenguards->GetGuardList()[1].stopAnimation)
	{
		guards2->Update(dt);
		guards2->m_anim->animActive = true;
	}
	castlenguards->UpdateCastlenGuards(dt,testPosition);

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

		for (int i = 0; i < apples->GetAppleVec().size(); i++)
		{
			if (apples->GetAppleVec()[i].spawned)
			{
				d.x = apples->GetAppleVec()[i].position.x - KSpos.pos.x;
				d.y = apples->GetAppleVec()[i].position.y - KSpos.pos.y;
			}
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
				apples->GetAppleVec()[i].spawned = false;
				Hunger = 100;
			}
		}
	}
}

void Scene1::Update(double dt)
{
	SceneBase::Update(dt);
	if (Application::IsKeyPressed('W'))
	{
		testPosition.y++;
	}
	if (Application::IsKeyPressed('A'))
	{
		testPosition.x--;

	}
	if (Application::IsKeyPressed('S'))
	{
		testPosition.y--;

	}
	if (Application::IsKeyPressed('D'))
	{
		testPosition.x++;
	}
	if (enemy->GetState() != Behavior::IDLE && enemy->GetState() != Behavior::RUN)
	{
		testPosition = enemy->GetPosition();
	}
	apples->UpdateApplesFSM(dt);
	CastleFSMUpdate(dt);
	HealPointFSMUpdate(dt);
	KingSlimeFSMUpdate(dt);
	enemy->Update(dt, 50, 300, 200, 290);

	float distance = (testPosition - Vector3(147, 371, 1)).LengthSquared();
	distancetoenemy = (castlenguards->GetGuardList()[0].position - enemy->GetPosition()).LengthSquared();
	distancetoenemy1 = (castlenguards->GetGuardList()[1].position - enemy->GetPosition()).LengthSquared();
	directionenemy = (enemy->GetPosition() - castlenguards->GetGuardList()[0].position).Normalize();
	directionenemy1 = (enemy->GetPosition() - castlenguards->GetGuardList()[1].position).Normalize();

	cout << distancetoenemy << endl;
	if (distancetoenemy < 2000 && castlenguards->GetState() != Castle::CLOSE|| distancetoenemy1 <2000 && castlenguards->GetState() != Castle::CLOSE)
	{
		enemy->SetState(Behavior::RUN);
		testPosition = testPosition + directionenemy * 1.5;
		enemy->SetPosition(testPosition);
	}
	
	float combineSRadSquare = (castleScale.x + 20) * (castleScale.y + 20);
	if (distance < 25000 && castlenguards->GetState() == Castle::OPEN)
	{
		castlenguards->SetState(Castle::DEFENCE);
	}
	else if (distance > 25000 && castlenguards->GetState()== Castle::DEFENCE)
	{
		castlenguards->SetState(Castle::OPEN);
	}
	//cout << distance << " " << testPosition.x << " " << testPosition.y << endl;

	//cout << enemy->GetPosition().x << " " << enemy->GetPosition().y << " " << enemy->GetState() << endl;
	fps = (float)(1.f / dt);
}

int Scene1::RandomInteger(int lowerLimit, int upperLimit)
{
	return rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
}

void Scene1::RenderFSM()
{
	for (int i = 0; i < castlenguards->GetGuardList().size(); i++)
	{
		switch (castlenguards -> GetGuardList()[i].guardState)
		{
		case Guards::IDLING:
			//cout << "IDLING" << endl;

			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
			break;
		case Guards::MOVINGD:
			//cout << "DOWN" << endl;

			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
			break;
		case Guards::MOVINGUP:
			//cout << "UP" << endl;

			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDSUP]);
			break;
		case Guards::MOVINGL:
			//cout << "LEFT" << endl;

			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDSL]);
			break;
		case Guards::MOVINGR:
			//cout << "RIGHT" << endl;

			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDSR]);
			break;
		case Guards::GUARDING:
			//cout << "RIGHT" << endl;

			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
			break;
		case Guards::ATTACKING:
			//cout << "RIGHT" << endl;

			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
			break;
		}
		Render2DMeshWScale(castlenguards->GetGuardList()[i].guardMesh->GetNewMesh(), false, castlenguards->GetGuardList()[i].scale.x, castlenguards->GetGuardList()[i].scale.y, castlenguards->GetGuardList()[i].position.x, castlenguards->GetGuardList()[i].position.y, false);

	}

	Render2DMeshWScale(meshList[GEO_CASTLE], false, 250, 250, castlePostion.x, castlePostion.y, false);

	Render2DMeshWScale(meshList[GEO_DOOR], false, 250, 250, castlenguards->GetDoorPos().x, castlenguards->GetDoorPos().y, false);
	Render2DMeshWScale(meshList[GEO_VILLAGER], false, 20, 20, enemy->GetPosition().x, enemy->GetPosition().y, false);

	
	for (int i = 0; i < apples->GetAppleVec().size(); i++)
	{
		switch (apples->GetAppleVec()[i].appleStates)
		{
		case  Apples::SPAWNING:
			apples->GetAppleVec()[i].appleMesh->SetNewMesh(meshList[GEO_APPLES]);
			break;
		case Apples::SPAWNED:
			apples->GetAppleVec()[i].appleMesh->SetNewMesh(meshList[GEO_APPLES]);
			break;
		case Apples::ROTTING:
			apples->GetAppleVec()[i].appleMesh->SetNewMesh(meshList[GEO_ROTTENAPPLE]);
			break;
		case Apples::DECAYED:
			apples->GetAppleVec()[i].appleMesh->SetNewMesh(meshList[GEO_DECAYAPPLE]);
			break;
		}
		if (apples->GetAppleVec()[i].spawned)
		{
			Render2DMeshWScale(apples->GetAppleVec()[i].appleMesh->GetNewMesh(), false, 30, 30, apples->GetAppleVec()[i].position.x, apples->GetAppleVec()[i].position.y, false);
		}
		//cout << i << " " << apples->GetAppleVec()[i].randomProb << "   " << apples->GetAppleVec()[i].probability << endl;
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
	ss << "Open: " << castlenguards->GetOpenCounter() << " Close: " << castlenguards->GetCloseCounter() << " Defence: " << castlenguards->GetDefenceCounter();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, 20, 520);

	switch (castlenguards->GetState())
	{
	case Castle::OPEN:
		ss.str("");
		ss << "OPEN";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 20, 540);
		break;
	case Castle::CLOSE:
		ss.str("");
		ss << "CLOSE";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 20, 540);
		break;
	case Castle::DEFENCE:
		ss.str("");
		ss << "DEFENCE";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 20, 540);
		break;
	}
	ss.str("");
	ss << "To next random state " << castlenguards->GetTimer();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, 20, 570);
	ss.str("");
	ss << "Previous random state number " << castlenguards->GetTempInt();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, 20, 585);
	switch (enemy->GetState())
	{
	case Behavior::IDLE:
		ss.str("");
		ss << "IDLING";
		break;
	case Behavior::RUN:
		ss.str("");
		ss << "RUN";
		break;
	case Behavior::GOTOWP:
		ss.str("");
		ss << "WALKING";
		break;
	default:
		break;
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, enemy->GetPosition().x, enemy->GetPosition().y - 20);



	ss.str("");
	ss << "KS Hunger: " << Hunger;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 500, 200);
	for (int i = 0; i < castlenguards->GetGuardList().size(); i++)
	{
		switch (castlenguards->GetGuardList()[i].guardState)
		{
		case Guards::IDLING:
			ss.str("");
			ss << "IDLING";
			break;
		case Guards::MOVINGD:
			//cout << "DOWN" << endl;
			ss.str("");
			ss << "DOWN";
			break;
		case Guards::MOVINGUP:
			ss.str("");
			ss << "UP";
			break;
		case Guards::MOVINGL:
			ss.str("");
			ss << "LEFT";
			break;
		case Guards::MOVINGR:
			ss.str("");
			ss << "RIGHT";
			break;
		case Guards::GUARDING:
			ss.str("");
			ss << "GUARD";
			break;
		case Guards::ATTACKING:
			ss.str("");
			ss << "ATTACK INTRUDER";
			break;
		}
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, castlenguards->GetGuardList()[i].position.x - 12, castlenguards->GetGuardList()[i].position.y - 16);
	}

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
	int x = 0;
	ostringstream oss;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, 180, 50);
	for (int i = 0; i < apples->GetAppleVec().size(); i++)
	{
		ss.str("");
		x -= 3;
		switch (apples->GetAppleVec()[i].appleStates)
		{
		case  Apples::SPAWNING:
			ss << "Apple " << i << " spawning " << apples->GetAppleVec()[i].timer;
			break;
		case Apples::SPAWNED:
			ss << "Apple " << i << " spawned " << apples->GetAppleVec()[i].despawn;
			break;
		case Apples::ROTTING:
			ss << "Apple " << i << " rotting " << apples->GetAppleVec()[i].despawn;
			break;
		case Apples::DECAYED:
			ss << "Apple " << i << " decaying " << apples->GetAppleVec()[i].despawn;
			break;
		default:
			break;
		}
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 1), 20, apples->GetAppleVec()[i].position.x-30, apples->GetAppleVec()[i].position.y - x);

	}
	int y = 0;
	
	ostringstream oss2;
	for (int i = 0; i < apples->GetAppleVec().size(); i++)
	{
		x += 20;
		ss.str("");

		if (apples->GetAppleVec()[i].timer > 0)
			ss << "Apple " << i << " " << apples->GetAppleVec()[i].timer;

		if (apples->GetAppleVec()[i].spawned && apples->GetAppleVec()[i].despawn> 10)
			ss << "Apple " << i << " Spawned";

		if (apples->GetAppleVec()[i].appleStates == Apples::ROTTING)
			ss << "Apple " << i << " Rotting";

		if (apples->GetAppleVec()[i].appleStates == Apples::DECAYED)
			ss << "Apple " << i << " Decaying";
		
		oss2.str("");
		oss2 << "Rot Probability " << apples->GetAppleVec()[i].probability;

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