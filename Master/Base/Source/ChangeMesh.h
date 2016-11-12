#pragma once

#include "Mesh.h"

class ChangeMesh
{
public:
	ChangeMesh();
	~ChangeMesh();
	void SetNewMesh(Mesh* newMesh);
	Mesh* GetNewMesh();

private:
	Mesh* mesh;

};