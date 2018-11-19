#include "ComponentMesh.h"
#include "GameObject.h"


ComponentMesh::ComponentMesh( GameObject* my_go)
{
	type = MESH;
	active = true;
	this->my_go = my_go;
	avaliable = false;
}

ComponentMesh::ComponentMesh(GameObject* my_go, const Mesh& mesh)
{
	type = MESH;
	active = true;
	this->my_go = my_go;
	this->mesh = mesh;
	avaliable = true;
}



ComponentMesh::~ComponentMesh()
{

	
}
