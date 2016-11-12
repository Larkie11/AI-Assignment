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
	srand((unsigned)time(NULL));
	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(Application::GetInstance().GetScreenHeight(), Application::GetInstance().GetScreenWidth(), 32);
	m_cMap->LoadMap("Data//MapData_WM2.csv");
	RandomInt = RandomInteger(500, 700);
	//cout << RandomInt << endl;
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

	for (int i = 0; i < 2; i++)
	{
		treePosition1.pos.Set(RandomInteger(230, 500), RandomInteger(50, 300), 1);
		treePositions.push_back(treePosition1);
	}
	for (int i = 0; i < RandomInteger(1, 5); i++)
	{
		int randTreePosition = RandomInteger(-40, 10);
		int randTree = RandomInteger(0, treePositions.size()-1);
		//cout << treePositions[randTree].pos.x << endl;
		applePositions1.timer = RandomInteger(300, 800);
		applePositions1.spawned = false;
		applePositions1.position.pos.x = treePositions[randTree].pos.x - randTreePosition;
		applePositions1.position.pos.y = treePositions[randTree].pos.y + 150;
		applePositions.push_back(applePositions1);
		cout << applePositions[i].timer << " " << applePositions1.position.pos.x << " " << applePositions1.position.pos.y << endl;
	}	
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
	for (int i = 0; i < applePositions.size(); i++)
	{
		applePositions[i].timer -= dt;
		//cout << applePositions[i].timer << endl;
		applePositions[i].newPosition.pos.y = applePositions[i].position.pos.y - 150;
		cout << i << " " << applePositions[i].newPosition.pos.y << endl;
		if (applePositions[i].timer <= 0 && !applePositions[i].spawned)
		{
			applePositions[i].spawned = true;
			applePositions[i].position.pos.y = applePositions[i].newPosition.pos.y;

		}
		/*if (applePositions[i].spawned)
		{
			if (applePositions[i].position.pos.y > applePositions[i].newPosition.pos.y)
			{
				applePositions[i].position.pos.y--;
			}
		}*/
	}
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
		if (castleState == CLOSE)
		if (guard1.position.pos.y != 320 || guard2.position.pos.x != 162)
		{
			guard1.position.pos.y = 320;
			guard2.position.pos.y = 360;
			guard2.position.pos.x = 162;
		}
	}
	//If even number, castle is open
	if (TempRandomInt % 2 == 0)
	{
		castleState = OPEN;
	}
	//Odd number castle is closed
	else
	{
		castleState = CLOSE;
	}
	//Animation and position of guard & door
	if (castleState == OPEN)
	{
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
				RandomInt -= dt*0.001;
			}
		}
	}
	if (castleState == CLOSE)
	{
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
				RandomInt -= dt*0.01;
			}
		}
	}
}
void Scene1::Update(double dt)
{
	SceneBase::Update(dt);
	CastleFSMUpdate(dt);
	SpawnAppleFSMUpdate(dt);
	fps = (float)(1.f / dt);

}
int Scene1::RandomInteger(int lowerLimit, int upperLimit)
{
	return rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
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
	Render2DMeshWScale(guard1.guardMesh->GetNewMesh(), false, guard1.scale.x, guard1.scale.y, guard1.position.pos.x, guard1.position.pos.y, false);
	Render2DMeshWScale(guard2.guardMesh->GetNewMesh(), false, guard2.scale.x, guard2.scale.y, guard2.position.pos.x, guard2.position.pos.y, false);

	Render2DMeshWScale(meshList[GEO_DOOR], false, 250, 250, doorPos.pos.x, doorPos.pos.y, false);
	
	Render2DMeshWScale(meshList[GEO_CASTLE], false, 250, 250, 30, 300, false); 
	
	for (int i = 0; i < treePositions.size(); i++)
	{
		Render2DMeshWScale(meshList[GEO_TREE], false, 170, 170, treePositions[i].pos.x, treePositions[i].pos.y, false);
	}
	for (int i = 0; i < applePositions.size(); i++)
	{
		if (applePositions[i].spawned)
		{
			Render2DMeshWScale(meshList[GEO_APPLES], false, 30, 30, applePositions[i].position.pos.x, applePositions[i].position.pos.y, false);
		}
	}
	std::ostringstream ss;
	//On screen text
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 30, 0, 0);
	ss.str("");
	if (castleState == OPEN)
	{
		ss << "OPEN";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 20, 550);
		ss.str("");
	}
	else
	{
		ss << "CLOSE";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 30, 20, 550);
		ss.str("");
	}
}

void Scene1::RenderGO()
{
}

void Scene1::Render()
{
	SceneBase::Render();
	RenderMap();
}

void Scene1::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}