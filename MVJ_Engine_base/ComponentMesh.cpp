#include "ComponentMesh.h"
#include "GameObject.h"


ComponentMesh::ComponentMesh( GameObject* my_go)
{
	type = MESH;
	active = true;
	this->my_go = my_go;
	avaliable = false;
	mesh.vertices = nullptr;
}

ComponentMesh::ComponentMesh(GameObject* my_go, const Mesh& mesh, float3* vertices)
{
	type = MESH;
	active = true;
	this->my_go = my_go;
	this->mesh = mesh;
	avaliable = true;
	this->mesh.vertices = vertices;
}



ComponentMesh::~ComponentMesh()
{	
	delete mesh.vertices;
}
