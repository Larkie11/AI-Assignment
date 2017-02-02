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
	shoot = new Bullet();
}

Scene1::~Scene1()
{
	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}
	delete apples;
	apples = NULL;
	delete castlenguards;
	castlenguards = NULL;
	delete enemy;
	enemy = NULL;
}

void Scene1::Reset()
{
}

void Scene1::Init()
{
	apples = new AppleSpawning();
	castlenguards = new CastlenGuards();
	enemy = new Enemy();
	SceneBase::Init();
	InitFSM();
	srand((unsigned)time(NULL));
	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(Application::GetInstance().GetScreenHeight(), Application::GetInstance().GetScreenWidth(), 32);
	m_cMap->LoadMap("Data//MapData_WM2.csv");
}

void Scene1::InitFSM()
{
	mess = new MessageBoard();
	//Heal Point data
	healpointState = IDLE;
	PP = 3;
	PPcounter = 100;
	healpointPos.pos.Set(450, 100, 1);

	//KingSlime data
	Hunger = 30;
	HungerCounter = 100;
	MoveCounter = 100;
	RandomInt2 = RandomInteger(1, 200);
	TempRandomInt2 = RandomInt2;
	RandomMoveX = RandomInteger(-5, 5);
	RandomMoveY = RandomInteger(-5, 5);
	KSstate = LAZE;
	KSpos.Set(500, 300, 1);
	KSHP = 45;
	KSHPcounter = 100;
	detect = false;
	detected = false;

	apples->Init(40);
	enemy->Init(Behavior::GOTOWP, Vector3(100, 100, 1), Vector3(100, 200, 1), 30);
	castlePostion.Set(30, 300, 1);
	testPosition.Set(30, 300, 1);
	castleScale.Set(250, 250, 1);
	castlenguards->InitCastlenGuards(80);
	directionenemy.SetZero();
	directionenemy1.SetZero();
	testPosition = enemy->GetPosition();
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
	apples->UpdateApplesFSM(dt);
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
	SpriteAnimation *archerR = dynamic_cast<SpriteAnimation*>(meshList[GEO_ARCHERR]);
	if (archerR && !castlenguards->GetArcher().stopAnimation)
	{
		archerR->Update(dt);
		archerR->m_anim->animActive = true;
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
	castlenguards->UpdateCastlenGuards(dt,KSpos);

	if (castlenguards->GetArcher().guardState == Guards::ATTACKING)
	{
		if (!shot)
		{
			if (castlenguards->GetArcher().position.x < KSpos.x)
			{
				float distancetoenemy2 = (castlenguards->GetArcher().position - KSpos).LengthSquared();
				//cout << "dist" << distancetoenemy2 << endl;
				shoot->SetPosition(castlenguards->GetArcher().position, distancetoenemy2 / 800);
			}
			else
			{
				float distancetoenemy2 = (KSpos - castlenguards->GetArcher().position).LengthSquared();
				//cout << "dist" << distancetoenemy2 << endl;
				shoot->SetPosition(castlenguards->GetArcher().position, -(distancetoenemy2 / 800));
			}
			shot = true;
		}
		shoot->Update(dt);
	}
}

void Scene1::HealPointFSMUpdate(double dt)
{
	Vector3 pos;
	pos.x = healpointPos.pos.x;
	pos.y = healpointPos.pos.y;
	bool KSdetected = Detect(pos, KSpos, HPRadius);

	SpriteAnimation *HealIDLE = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEAL_IDLE]);
	if (HealIDLE)
	{
		HealIDLE->Update(dt);
		HealIDLE->m_anim->animActive = true;
	}
	SpriteAnimation *HealHEAL = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEAL_HEAL]);
	if (HealHEAL)
	{
		HealHEAL->Update(dt);
		HealHEAL->m_anim->animActive = true;
	}

	RandomInt2 -= dt* 0.001;
	if (RandomInt2 <= 0)
	{
		RandomInt2 = RandomInteger(1, 200);
		TempRandomInt2 = RandomInt2;
	}

	//HealPoint FSM
	if (healpointState == IDLE)
	{
		if (KSdetected)
		{
			mess->SetFromLabel("HealPoint");
			mess->SetToLabel("KingSlime");
			mess->SetMessage("HEALING");
			healpointState = HEAL;
		}
		else
			healpointState = IDLE;
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

	bool enemyDetected = Detect(KSpos, castlenguards->GetGuardList()[0].position, KSRadius);

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
	if ((KSstate != EAT || KSstate != ATTACK) && MoveCounter <= 0)
	{
		MoveCounter = 300;
		Hunger--;
		if (Hunger == 0)
			KSstate = EAT;
		RandomMoveX = RandomInteger(-50, 50);
		RandomMoveY = RandomInteger(-50, 50);
		if (enemyDetected)
		{
			KSstate = ATTACK;
			if (KSHP <= 40)
			{
				KSstate = RUN;
			}
		}
	}

	if ((KSstate != EAT || KSstate != ATTACK) && TempRandomInt2 % 2 == 0)
	{
		KSstate = LAZE;
		if (enemyDetected)
		{
			KSstate = ATTACK;
			if (KSHP <= 40)
			{
				KSstate = RUN;
			}
		}
	}
	else if (Hunger == 0)
	{
		KSstate = EAT;
		if (enemyDetected)
		{
			KSstate = ATTACK;
			if (KSHP <= 40)
			{
				KSstate = RUN;
			}
		}
	}
	else
	{
		KSstate = MOVE;
	}
	if (KSstate == ATTACK)
	{
		KSHPcounter -= 1 * dt;
		if (KSHPcounter <= 0)
		{
			KSHP--;
			KSHPcounter = 100;
			if (KSHP <= 40)
			{
				KSstate = RUN;
			}
		}
	}

	switch (KSstate)
	{
	case KingSlime::MOVE:
	{
							if (KSpos.x >= 0 && KSpos.x <= 700)
							{
								KSpos.x += RandomMoveX * dt;
							}
							if (KSpos.y >= 0 && KSpos.y <= 300)
							{
								KSpos.y += RandomMoveY * dt;
							}

							if (enemyDetected)
							{
								KSstate = ATTACK;
							}
	}
		break;
	case KingSlime::ATTACK:
	{
							mess->SetFromLabel("KingSlime");
							mess->SetToLabel("Guards");
							mess->SetMessage("EARTHQUAKE!");
							bool detected = true;

	}
		break;
	case KingSlime::RUN:
	{
						   if (healpointState != HEAL)
						   {
							   mess->SetFromLabel("KingSlime");
							   mess->SetToLabel("HealPoint");
							   mess->SetMessage("HEAL ME!!");
						   }
						   Vector3 d;
						   d.x = healpointPos.pos.x - KSpos.x;
						   d.y = healpointPos.pos.y - KSpos.y;
						   if (KSpos.x != healpointPos.pos.x)
						   {
							   KSpos.x += d.x * dt * 0.5f;
							   KSpos.y += d.y * dt * 0.5f;
							   if (KSpos.x >= healpointPos.pos.x - 1 && KSpos.x <= healpointPos.pos.x + 1)
							   {
								   KSstate = LAZE;
							   }
						   }
						   if (d.IsZero())
						   {
							   return;
						   }
	}
		break;
	case KingSlime::EAT:
	{
						   Vector3 d;

						   for (int i = 0; i < apples->GetAppleVec().size(); i++)
						   {
							   if (apples->GetAppleVec()[1].spawned)
							   {
								   d.x = apples->GetAppleVec()[1].position.x - KSpos.x;
								   d.y = apples->GetAppleVec()[1].position.y - KSpos.y;
								   if (KSpos.x != apples->GetAppleVec()[1].position.x)
								   {

									   KSpos.x += d.x * dt * 0.2f;
									   KSpos.y += d.y * dt * 0.2f;
									   if (KSpos.x >= apples->GetAppleVec()[1].position.x - 1 && KSpos.x <= apples->GetAppleVec()[1].position.x + 1)
									   {
										   apples->GetAppleVec()[i].spawned = false;
										   apples->SetAppleDespawn(0, 1);
										   Hunger = 100;
										   KSstate = LAZE;
									   }
								   }
							   }
							   else
								   KSstate = LAZE;
							   if (d.IsZero())
							   {
								   return;
							   }
						   }
	}
		break;
	}
}

float GetDistance(float x1, float y1, float x2, float y2) { return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); } // OK

bool Scene1::Detect(Vector3 pos1, Vector3 pos2, float radius)
{
	bool detect = false;
	float distance = GetDistance(pos1.x, pos1.y, pos2.x, pos2.y);
	if (distance <= radius) detect = true;
	return detect;
}

bool Scene1::HPDetect(Vector3 pos1, Vector3 pos2, float radius)
{
	bool detect = false;
	float distance = GetDistance(pos1.x, pos1.y, pos2.x, pos2.y);
	if (distance <= radius) detect = true;
	return detect;
}

void Scene1::Update(double dt)
{
	SceneBase::Update(dt);
	UpdateFSM(dt);
	if (Application::IsKeyPressed('W'))
	{
		castlenguards->MinusHealth(2, 0);
	}
	if (Application::IsKeyPressed('S'))
	{
		castlenguards->SetHealth(100,0);

	}
	if (Application::IsKeyPressed('A'))
	{
		castlenguards->MinusHealth(2, 1);
	}
	if (Application::IsKeyPressed('D'))
	{
		castlenguards->SetHealth(100, 1);

	}		
	
	//cout << shoot->vel << " " << shoot->pos << " " << shoot->gravity << endl;

	if (shoot->GetPosition().y < KSpos.y)
	{
		shot = false;
	}
	fps = (float)(1.f / dt);
}

void Scene1::UpdateFSM(double dt)
{
	if (enemy->GetState() != Behavior::IDLE && enemy->GetState() != Behavior::RUN)
	{
		testPosition = enemy->GetPosition();
	}
	if (castlenguards->GetGuardList()[0].health <= 0)
		castlenguards->GetGuardList()[0].health = 100;
	else
	{

	}

	CastleFSMUpdate(dt);
	

	HealPointFSMUpdate(dt);
	KingSlimeFSMUpdate(dt);
	SpawnAppleFSMUpdate(dt);
	enemy->Update(dt, 50, 300, 200, 290);

	float distance = (KSpos - Vector3(147, 371, 1)).LengthSquared();
	distancetoenemy = (castlenguards->GetGuardList()[0].position - KSpos).LengthSquared();
	distancetoenemy1 = (castlenguards->GetGuardList()[1].position - KSpos).LengthSquared();

	if (directionenemy!= Vector3( 0,0,0))
	directionenemy = (KSpos - castlenguards->GetGuardList()[0].position).Normalize();
	if (directionenemy1 != Vector3(0, 0, 0))
	directionenemy1 = (KSpos - castlenguards->GetGuardList()[1].position).Normalize();

	if (distancetoenemy < 2000 && castlenguards->GetState() != Castle::CLOSE || distancetoenemy1 <2000 && castlenguards->GetState() != Castle::CLOSE)
	{
		//enemy->SetState(Behavior::RUN);
		//testPosition = testPosition + directionenemy * 1.5;
		//enemy->SetPosition(testPosition);
	}

	float combineSRadSquare = (castleScale.x + 20) * (castleScale.y + 20);
	if (distance < 1000000 && castlenguards->GetState() == Castle::OPEN)
	{
		castlenguards->SetState(Castle::DEFENCE);
	}
	else if (distance > 1000000 && castlenguards->GetState() == Castle::DEFENCE)
	{
		castlenguards->SetState(Castle::OPEN);
	}
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
			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
			break;
		case Guards::MOVINGD:
			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
			break;
		case Guards::MOVINGUP:
			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDSUP]);
			break;
		case Guards::MOVINGL:
			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDSL]);
			break;
		case Guards::MOVINGR:
			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDSR]);
			break;
		case Guards::GUARDING:
			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
			break;
		case Guards::ATTACKING:
			castlenguards->GetGuardList()[i].guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
			break;
		}
		Render2DMeshWScale(castlenguards->GetGuardList()[i].guardMesh->GetNewMesh(), false, castlenguards->GetGuardList()[i].scale.x, castlenguards->GetGuardList()[i].scale.y, castlenguards->GetGuardList()[i].position.x, castlenguards->GetGuardList()[i].position.y, false);
	}
	switch (castlenguards->GetArcher().guardState)
	{
	case Guards::IDLING:
		castlenguards->GetArcher().guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
		break;
	case Guards::MOVINGOUT:
		castlenguards->GetArcher().guardMesh->SetNewMesh(meshList[GEO_ARCHERR]);
		break;
	case Guards::ATTACKING:
		castlenguards->GetArcher().guardMesh->SetNewMesh(meshList[GEO_ARCHERATT]);
		break;
	}
	Render2DMeshWScale(meshList[GEO_DOOR], false, 250, 250, castlenguards->GetDoorPos().x, castlenguards->GetDoorPos().y, false);
	Render2DMeshWScale(meshList[GEO_CASTLE], false, 250, 250, castlePostion.x, castlePostion.y, false);
	

	if (castlenguards->GetArcher().guardState == Guards::ATTACKING)
		Render2DMeshWScale(meshList[GEO_APPLES], false, castlenguards->GetArcher().scale.x, castlenguards->GetArcher().scale.y, shoot->GetPosition().x, shoot->GetPosition().y, false);


	if (castlenguards->GetArcher().guardState != Guards::IDLING)
	Render2DMeshWScale(castlenguards->GetArcher().guardMesh->GetNewMesh(), false, castlenguards->GetArcher().scale.x, castlenguards->GetArcher().scale.y, castlenguards->GetArcher().position.x, castlenguards->GetArcher().position.y, false);

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
		Render2DMeshWScale(meshList[GEO_HEAL_HEAL], false, 70, 70, healpointPos.pos.x, healpointPos.pos.y, false);
	else if (healpointState == REST)
		Render2DMeshWScale(meshList[GEO_HEAL_REST], false, 70, 70, healpointPos.pos.x, healpointPos.pos.y, false);
	else
		Render2DMeshWScale(meshList[GEO_HEAL_IDLE], false, 70, 70, healpointPos.pos.x, healpointPos.pos.y, false);

	//Render Kingslime
	if (KSstate == IDLE)
		Render2DMeshWScale(meshList[GEO_KSIDLE], false, 80, 50, KSpos.x - 30, KSpos.y - 20, false);
	else if (KSstate == MOVE || KSstate == EAT || KSstate == ATTACK || KSstate == RUN)
	{
		if (RandomMoveX < 0)
			Render2DMeshWScale(meshList[GEO_KSMOVEL], false, 100, 80, KSpos.x - 30, KSpos.y - 20, false);
		else
			Render2DMeshWScale(meshList[GEO_KSMOVER], false, 100, 80, KSpos.x - 30, KSpos.y - 20, false);
	}
	//Render2DMeshWScale(meshList[GEO_KSRADIUS], false, KSRadius, KSRadius, KSpos.x, KSpos.y, false);
	Render2DMeshWScale(meshList[GEO_VILLAGER], false, 20, 20, enemy->GetPosition().x, enemy->GetPosition().y, false);

	/*if (detected)
	{
		Render2DMeshWScale(meshList[GEO_KSDMG], false, 200, 200, castlenguards->GetGuardList()[0].position.x, castlenguards->GetGuardList()[0].position.y, false);
	}*/
}

void Scene1::RenderFSMText()
{
	std::ostringstream ss;
	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 30, 0, 0);

	switch (castlenguards->GetState())
	{
	case Castle::OPEN:
		ss.str("");
		ss << "CASTLE STATE: OPEN";
		break;
	case Castle::CLOSE:
		ss.str("");
		ss << "CASTLE STATE: CLOSE";
		break;
	case Castle::DEFENCE:
		ss.str("");
		ss << "CASTLE STATE: DEFENCE";
		break;
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 0, 560);

	ss.str("");
	ss << "To next random state timer " << castlenguards->GetTimer();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, 0, 583);
	ss.str("");
	ss << "Open: " << castlenguards->GetOpenCounter() << " Close: " << castlenguards->GetCloseCounter() << " Defence: " << castlenguards->GetDefenceCounter();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7, 0.2, 0), 20, 0, 530);

	/*ss.str("");
	ss << "Previous random state number " << castlenguards->GetTempInt();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, 20, 585);*/
	ss.str("");
	ss << "Probability to open: " << castlenguards->GetOpenProb();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.5, 0, 0.2), 20, 15, 548);
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
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 20, enemy->GetPosition().x, enemy->GetPosition().y - 20);

	ss.str("");
	ss << "KS HP: " << KSHP;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, KSpos.x, KSpos.y - 10);
	for (int i = 0; i < castlenguards->GetGuardList().size(); i++)
	{
		switch (castlenguards->GetGuardList()[i].guardState)
		{
		case Guards::IDLING:
			ss.str("");
			ss << "IDLING";
			break;
		case Guards::MOVINGD:
		case Guards::MOVINGUP:
		case Guards::MOVINGL:
		case Guards::MOVINGR:
			ss.str("");
			if (castlenguards->GetState() == Castle::OPEN)
			{
				ss << "MOVING OUT";
			}
			else if (castlenguards->GetState() == Castle::DEFENCE)
			{
				ss << "MOVING TO TARGET";
			}
			else if (castlenguards->GetState() == Castle::CLOSE)
			{
				ss << "MOVING IN";
			}
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
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, healpointPos.pos.x, healpointPos.pos.y - 35);
		break;
	case IDLE:
		ss.str("");
		ss << "IDLING";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, healpointPos.pos.x, healpointPos.pos.y - 35);
		break;
	case HEAL:
		ss.str("");
		ss << "HEALING PLAYER";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, healpointPos.pos.x, healpointPos.pos.y - 35);
		break;
	}
	switch (KSstate)
	{
	case Scene1::LAZE:
		ss.str("");
		ss << "KS State:IDLING";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, KSpos.x, KSpos.y - 30);
		break;
	case Scene1::MOVE:
		ss.str("");
		ss << "KS State:MOVING";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, KSpos.x, KSpos.y - 30);
		break;
	case Scene1::ATTACK:
		ss.str("");
		ss << "KS State:ATTACKING";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, KSpos.x, KSpos.y - 30);
		break;
	case Scene1::RUN:
		ss.str("");
		ss << "KS State:RUNNINIG";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, KSpos.x, KSpos.y - 30);
		break;
	case Scene1::EAT:
		ss.str("");
		ss << "KS State:EATING";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, KSpos.x, KSpos.y - 30);
		break;
	default:
		break;
	}

	ss.str("");
	ss << "PP: " << PP;
	int x = 0;
	ostringstream oss;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 20, healpointPos.pos.x, healpointPos.pos.y - 15);
	for (int i = 0; i < apples->GetAppleVec().size(); i++)
	{
		ss.str("");
		x -= 9;
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
		//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 1), 20, apples->GetAppleVec()[i].position.x-50, apples->GetAppleVec()[i].position.y - x);

	}
	ss.str("");
	ss << "Decayed apples:" <<  apples->countNoRot << "  Rotten apples:" << apples->countRot;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7, 0.2, 0), 20, 460, 550);

	ss.str("");
	ss << "Apples Rot Probability " << apples->GetAppleVec()[0].probability;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.5, 0, 0.2), 20, 500, 570);

	ss.str("");
	ss << "From : " << castlenguards->GetMessageBoard()->GetFromLabel() << "  To : " << castlenguards->GetMessageBoard()->GetToLabel() << "  M sg : " << castlenguards->GetMessageBoard()->GetMsg();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7, 0.1, 0.9), 20, 0, 50);

	ss.str("");
	ss << "From : " << mess->GetFromLabel() << "  To : " << mess->GetToLabel() << "  M sg : " << mess->GetMsg();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.7, 0.1, 0.9), 20, 0, 10);
}


void Scene1::RenderMap()
{
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
	delete castlenguards;
	delete enemy;
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}