#pragma once

#include "Mesh.h"

class ChangeMesh
{
public:
	ChangeMesh();
	~ChangeMesh();
	void SetNewMesh(Mesh* newMesh);
	Mesh* GetNewMesh();
	void SetFlip(bool flip);
	bool GetFlip();

private:
	Mesh* mesh;
	bool flip;

};