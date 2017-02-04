#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	//glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
//	meshList[GEO_TILESET1] = MeshBuilder::GenerateTileSet("GEO_TILESET", 80, 8);
	//meshList[GEO_TILESET1]->textureID = LoadTGA("Image//moderntileset.tga");

	meshList[GEO_TILESET1] = MeshBuilder::GenerateTileSet("GEO_TILESET3", 30, 30);
	meshList[GEO_TILESET1]->textureID = LoadTGA("Image//tileSet3.tga");
	meshList[GEO_FIREBALL] = MeshBuilder::Generate2DMesh("GEO_FIREBALL", Color(1, 1, 1), 100, 100, 1, 1);
	meshList[GEO_FIREBALL]->textureID = LoadTGA("Image//Fireball.tga");
	meshList[GEO_CASTLE] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 2);
	meshList[GEO_CASTLE]->textureID = LoadTGA("Image//Castle.tga");
	meshList[GEO_DOOR] = MeshBuilder::Generate2DMesh("sprite", Color(1, 1, 1), 100, 100, 1, 1);
	meshList[GEO_DOOR]->textureID = LoadTGA("Image//Door.tga");
	meshList[GEO_VILLAGER] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 3);
	meshList[GEO_VILLAGER]->textureID = LoadTGA("Image//Villager.tga");

	meshList[GEO_GUARDS] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 3);
	meshList[GEO_GUARDS]->textureID = LoadTGA("Image//Guards.tga");
	
	meshList[GEO_GUARDSL] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 3);
	meshList[GEO_GUARDSL ]->textureID = LoadTGA("Image//GuardsL.tga");
	meshList[GEO_GUARDSR] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 3);
	meshList[GEO_GUARDSR]->textureID = LoadTGA("Image//GuardsR.tga");
	meshList[GEO_GUARDSUP] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 3);
	meshList[GEO_GUARDSUP]->textureID = LoadTGA("Image//GuardsUP.tga");
	meshList[GEO_TREE] = MeshBuilder::Generate2DMesh("sprite", Color(1, 1, 1), 100, 100, 1, 1);
	meshList[GEO_TREE]->textureID = LoadTGA("Image//AppleTree.tga");
	meshList[GEO_APPLES] = MeshBuilder::Generate2DMesh("sprite", Color(1, 1, 1), 100, 100, 1, 1);
	meshList[GEO_APPLES]->textureID = LoadTGA("Image//Apple.tga");
	meshList[GEO_ROTTENAPPLE] = MeshBuilder::Generate2DMesh("sprite", Color(1, 1, 1), 100, 100, 1, 1);
	meshList[GEO_ROTTENAPPLE]->textureID = LoadTGA("Image//AppleRot.tga");
	meshList[GEO_DECAYAPPLE] = MeshBuilder::Generate2DMesh("sprite", Color(1, 1, 1), 100, 100, 1, 1);
	meshList[GEO_DECAYAPPLE]->textureID = LoadTGA("Image//AppleDecay.tga");
	meshList[GEO_ARCHERR] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 3);
	meshList[GEO_ARCHERR]->textureID = LoadTGA("Image//ArcherR.tga");
	meshList[GEO_ARCHERL] = MeshBuilder::GenerateSpriteAnimation("sprite", 1, 3);
	meshList[GEO_ARCHERL]->textureID = LoadTGA("Image//ArcherL.tga");
	meshList[GEO_ARCHERATT] = MeshBuilder::Generate2DMesh("sprite", Color(1, 1, 1), 100, 100, 1, 1);
	meshList[GEO_ARCHERATT]->textureID = LoadTGA("Image//ArcherAttack.tga");
	SpriteAnimation *castle = dynamic_cast<SpriteAnimation*>(meshList[GEO_CASTLE]);
	if (castle)
	{
		castle->m_anim = new Animation();
		castle->m_anim->Set(0, 1, 1, 1, true);
	}

	SpriteAnimation *guards = dynamic_cast<SpriteAnimation*>(meshList[GEO_GUARDS]);
	if (guards)
	{
		guards->m_anim = new Animation();
		guards->m_anim->Set(0, 2, 1, 1, true);
	}
	SpriteAnimation *guardsL = dynamic_cast<SpriteAnimation*>(meshList[GEO_GUARDSL]);
	if (guardsL)
	{
		guardsL->m_anim = new Animation();
		guardsL->m_anim->Set(0, 2, 1, 1, true);
	}
	SpriteAnimation *guardsR = dynamic_cast<SpriteAnimation*>(meshList[GEO_GUARDSR]);
	if (guardsR)
	{
		guardsR->m_anim = new Animation();
		guardsR->m_anim->Set(0, 2, 1, 1, true);
	}
	SpriteAnimation *guardsUP = dynamic_cast<SpriteAnimation*>(meshList[GEO_GUARDSUP]);
	if (guardsUP)
	{
		guardsUP->m_anim = new Animation();
		guardsUP->m_anim->Set(0, 2, 1, 1, true);
	}
	SpriteAnimation *archerR = dynamic_cast<SpriteAnimation*>(meshList[GEO_ARCHERR]);
	if (archerR)
	{
		archerR->m_anim = new Animation();
		archerR->m_anim->Set(0, 2, 1, 1, true);
	}
	SpriteAnimation *archerL = dynamic_cast<SpriteAnimation*>(meshList[GEO_ARCHERL]);
	if (archerL)
	{
		archerL->m_anim = new Animation();
		archerL->m_anim->Set(0, 2, 1, 1, true);
	}
	SpriteAnimation *villager = dynamic_cast<SpriteAnimation*>(meshList[GEO_VILLAGER]);
	if (villager)
	{
		villager->m_anim = new Animation();
		villager->m_anim->Set(0, 2, 1, 1, true);
	}

	meshList[GEO_HEAL_IDLE] = MeshBuilder::GenerateSpriteAnimation("Healing Fountain", 1, 4);
	meshList[GEO_HEAL_IDLE]->textureID = LoadTGA("Image//Fountain.tga");
	meshList[GEO_HEAL_REST] = MeshBuilder::GenerateSpriteAnimation("Empty Fountain", 1, 1);
	meshList[GEO_HEAL_REST]->textureID = LoadTGA("Image//EmptyFountain.tga");
	meshList[GEO_HEAL_HEAL] = MeshBuilder::GenerateSpriteAnimation("Healing Fountain", 1, 4);
	meshList[GEO_HEAL_HEAL]->textureID = LoadTGA("Image//Fountain.tga");
	meshList[GEO_HEAL_EFFECT] = MeshBuilder::GenerateSpriteAnimation("Healing Effect", 1, 9);
	meshList[GEO_HEAL_EFFECT]->textureID = LoadTGA("Image//HealEffect.tga");

	meshList[GEO_KSIDLE] = MeshBuilder::GenerateSpriteAnimation("Idle Slime", 1, 2);
	meshList[GEO_KSIDLE]->textureID = LoadTGA("Image//KingSlimeIdle.tga");
	meshList[GEO_KSMOVEL] = MeshBuilder::GenerateSpriteAnimation("Move Slime", 1, 7);
	meshList[GEO_KSMOVEL]->textureID = LoadTGA("Image//KingSlimeMove.tga");
	meshList[GEO_KSMOVER] = MeshBuilder::GenerateSpriteAnimation("Move Slime", 1, 7);
	meshList[GEO_KSMOVER]->textureID = LoadTGA("Image//KingSlimeMoveR.tga");
	meshList[GEO_KSATTACKFORCE] = MeshBuilder::GenerateSpriteAnimation("KingSlimeAttack", 2, 5);
	meshList[GEO_KSATTACKFORCE]->textureID = LoadTGA("Image//KSAttack.tga");
	meshList[GEO_KSATTACK] = MeshBuilder::GenerateSpriteAnimation("Move Slime", 1, 7);
	meshList[GEO_KSATTACK]->textureID = LoadTGA("Image//KingSlimeMove.tga");

	SpriteAnimation *ksIDLE = dynamic_cast<SpriteAnimation*>(meshList[GEO_KSIDLE]);
	if (ksIDLE)
	{
		ksIDLE->m_anim = new Animation();
		ksIDLE->m_anim->Set(0, 1, 1, 1, true);
	}
	SpriteAnimation *ksMOVEL = dynamic_cast<SpriteAnimation*>(meshList[GEO_KSMOVEL]);
	if (ksMOVEL)
	{
		ksMOVEL->m_anim = new Animation();
		ksMOVEL->m_anim->Set(0, 6, 1, 1, true);
	}
	SpriteAnimation *ksMOVER = dynamic_cast<SpriteAnimation*>(meshList[GEO_KSMOVER]);
	if (ksMOVER)
	{
		ksMOVER->m_anim = new Animation();
		ksMOVER->m_anim->Set(0, 6, 1, 1, true);
	}

	SpriteAnimation *HealIDLE = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEAL_IDLE]);
	if (HealIDLE)
	{
		HealIDLE->m_anim = new Animation();
		HealIDLE->m_anim->Set(0, 3, 1, 1, true);
	}
	SpriteAnimation *HealHEAL = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEAL_HEAL]);
	if (HealHEAL)
	{
		HealHEAL->m_anim = new Animation();
		HealHEAL->m_anim->Set(0, 3, 1, 1, true);
	}
	SpriteAnimation *HealEFFECT = dynamic_cast<SpriteAnimation*>(meshList[GEO_HEAL_EFFECT]);
	if (HealEFFECT)
	{
		HealEFFECT->m_anim = new Animation();
		HealEFFECT->m_anim->Set(0, 8, 1, 1, true);
	}
	SpriteAnimation *ksATTACK = dynamic_cast<SpriteAnimation*>(meshList[GEO_KSATTACK]);
	if (ksATTACK)
	{
		ksATTACK->m_anim = new Animation();
		ksATTACK->m_anim->Set(0, 6, 1, 1, true);
	}
	SpriteAnimation *KSAttackDust = dynamic_cast<SpriteAnimation*>(meshList[GEO_KSATTACKFORCE]);
	if (KSAttackDust)
	{
		KSAttackDust->m_anim = new Animation();
		KSAttackDust->m_anim->Set(0, 9, 1, 1, true);
	}

	Math::InitRNG();
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
}

void SceneBase::RenderTile(Mesh* mesh, unsigned tileID, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, 1);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	mesh->Render(tileID * 6, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneBase::RenderTileMap(Mesh* mesh, CMap* map)
{
	for (int y = 0; y < map->theNumOfTiles_Height; ++y)
	{
		for (int x = 0; x < map->theNumOfTiles_Width; ++x)
		{
			//if (map->theMap[y][x].BlockID != 0)
			{
				RenderTile(mesh, map->theMap[y][x].BlockID, 32, x * map->GetTileSize() - MapOffset.x, y * map->GetTileSize() - MapOffset.y);
			}
		}
	}
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	//glEnable(GL_DEPTH_TEST);
}

void SceneBase::Render2DMeshWScale(Mesh *mesh, const bool enableLight, const float sizeX, const float sizeY, const float x, const float y, const bool flip, const float offset)
{
	glDisable(GL_CULL_FACE);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	if (flip)
	{
		modelStack.Translate(x + offset, y, 0);
	}
	else
	{
		modelStack.Translate(x, y, 0);
	}
	modelStack.Scale(sizeX, sizeY, 1);

	if (flip)
		modelStack.Rotate(180, 0, 1, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_CULL_FACE);
}
void SceneBase::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	if (flip)
		modelStack.Translate(x + 32, y, 0);
	else
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	if (flip)
		modelStack.Rotate(180, 0, 1, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneBase::RenderBackground(Mesh* mesh)
{
	Render2DMesh(mesh, false, 1.0f,0,0,false); // World Overlay Background
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	//// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	//// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

void SceneBase::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if (Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if (Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	camera.Update(dt);
}

void SceneBase::Exit()
{
	//// Cleanup VBO
	//for (int i = 0; i < NUM_GEOMETRY; ++i)
	//{
	//	if (meshList[i])
	//		delete meshList[i];
	//}
	//glDeleteProgram(m_programID);
	//glDeleteVertexArrays(1, &m_vertexArrayID);
}
