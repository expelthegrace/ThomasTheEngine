#include "ComponentMesh.h"
#include "GameObject.h"


ComponentMesh::ComponentMesh( GameObject* my_go)
{
	type = MESH;
	active = true;
	this->my_go = my_go;
	avaliable = false;
}

ComponentMesh::ComponentMesh(GameObject* my_go, const Mesh& mesh,const std::vector<float3> & vertices)
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
}
