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
	cout << RandomInt;
	TempRandomInt = RandomInt;
	castleState = CLOSE;
	doorPos.pos.Set(30, 300, 1);
	guardPos.pos.Set(155, 320, 1);
	guardScale.x = 0.1;
	guardScale.y = 0.1;
	guardMesh = new ChangeMesh();
	guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
	stopGuardAnim = false;

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
void Scene1::FMSUpdate(double dt)
{
	SpriteAnimation *castle = dynamic_cast<SpriteAnimation*>(meshList[GEO_CASTLE]);
	if (castle)
	{
		castle->Update(dt);
		castle->m_anim->animActive = true;
	}
	SpriteAnimation *guards = dynamic_cast<SpriteAnimation*>(guardMesh->GetNewMesh());
	if (guards && !stopGuardAnim)
		{
			guards->Update(dt);
			guards->m_anim->animActive = true;
		}

	RandomInt -= dt*0.001;
	if (RandomInt <= 0)
	{
		RandomInt = RandomInteger(300, 400);
		TempRandomInt = RandomInt;
	}
	if (TempRandomInt % 2 == 0)
	{
		castleState = OPEN;
		
	}
	else
	{
		castleState = CLOSE;
	}
	if (castleState == OPEN)
	{
		if (doorPos.pos.y <= 350)
		{
			doorPos.pos.y++;
		}
		if (guardScale.x <= 30)
		{
			guardScale.x++;
			guardPos.pos.x -= 0.5;
		}
		if (guardScale.y <= 30)
		{
			guardScale.y++;
		}
		if (guardPos.pos.y >= 270)
		{
			guardPos.pos.y -= 0.5;
			guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
		}
		else
		{
			guardMoveLeft = true;
			guardMesh->SetNewMesh(meshList[GEO_GUARDSL]);
			if (guardPos.pos.x >= 25)
				guardPos.pos.x--;
			else
			{
				guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
				stopGuardAnim = true;
			}
		}
	}
	if (castleState == CLOSE)
	{
		if (guardPos.pos.x <= 140)
		{
			guardPos.pos.x++;
			guardMesh->SetNewMesh(meshList[GEO_GUARDSR]);
			stopGuardAnim = false;
		}
		else if(guardPos.pos.y <= 320)
		{
			guardPos.pos.y++;
			guardMesh->SetNewMesh(meshList[GEO_GUARDS]);
		}
		else if (doorPos.pos.y >= 300)
		{
			doorPos.pos.y--;
		}
	}
}
void Scene1::Update(double dt)
{
	SceneBase::Update(dt);
	FMSUpdate(dt);
	fps = (float)(1.f / dt);

	cout << RandomInt << endl;
	if (Application::IsKeyPressed(VK_F2))
	{
		RandomInt = RandomInteger(1, 100);
	}
	
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
	Render2DMeshWScale(guardMesh->GetNewMesh(), false, guardScale.x, guardScale.y, guardPos.pos.x, guardPos.pos.y, false);
	Render2DMeshWScale(meshList[GEO_DOOR], false, 250, 250, doorPos.pos.x, doorPos.pos.y, false);
	Render2DMeshWScale(meshList[GEO_CASTLE], false, 250, 250, 30, 300, false);

	//RenderBackground(meshList[GEO_CASTLE]);

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