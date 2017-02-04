#ifndef SCENEBASE_H
#define SCENEBASE_H
#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "CharacterData.h"
#include "Map.h"

class SceneBase : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_BLEND,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:
	static enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_BACKGROUND,
		GEO_TILESET1,
		GEO_TEXT,
		GEO_CASTLE,
		GEO_DOOR, 
		GEO_GUARDS, GEO_GUARDSL, GEO_GUARDSR, GEO_GUARDSUP,
		GEO_TREE,
		GEO_APPLES, GEO_ROTTENAPPLE, GEO_DECAYAPPLE,
		GEO_VILLAGER,
		GEO_ARCHERR,
		GEO_ARCHERL,
		GEO_FIREBALL,
		GEO_ARCHERATT,
		GEO_HEAL_IDLE,
		GEO_HEAL_HEAL,
		GEO_HEAL_REST,
		GEO_HEAL_EFFECT,

		GEO_KSMOVEL,
		GEO_KSMOVER,
		GEO_KSIDLE,
		GEO_KSATTACK,
		GEO_KSATTACKFORCE,

		NUM_GEOMETRY,
	};
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderTile(Mesh* mesh, unsigned tileID, float size, float x, float y);
	void RenderTileMap(Mesh* mesh, CMap* map);
	void RenderBackground(Mesh* mesh);
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size = 1.0f, const float x = 0.0f, const float y = 0.0f, const bool flip = false);
	void Render2DMeshWScale(Mesh *mesh, const bool enableLight, const float sizeX, const float sizeY, const float x = 0.0f, const float y = 0.0f, const bool flip = false, const float offset = 0);

protected:
	Vector3 MapOffset;
	CharacterData FontData;
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	MS modelStack;
	MS viewStack;
	MS projectionStack;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Camera3 camera;
	Light lights[2];
	float fps;
	float xtranslate;
	float ytranslate;

};

#endif // !SCENEBASE_H
