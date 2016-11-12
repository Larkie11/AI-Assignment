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