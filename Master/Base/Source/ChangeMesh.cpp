#include "ChangeMesh.h"

ChangeMesh::ChangeMesh() :mesh(NULL)
{

}

ChangeMesh::~ChangeMesh()
{

}

void ChangeMesh::SetNewMesh(Mesh* newMesh)
{
	mesh = newMesh;
}

Mesh* ChangeMesh::GetNewMesh()
{
	return mesh;
}

void ChangeMesh::SetFlip(bool flip)
{
	this->flip = flip;
}
bool ChangeMesh::GetFlip()
{
	return flip;
}