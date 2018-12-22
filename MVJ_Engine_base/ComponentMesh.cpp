#include "ComponentMesh.h"
#include "GameObject.h"
#include "Application.h"
#include "JSONManager.h"

ComponentMesh::ComponentMesh( GameObject* my_go)
{
	UID = App->generateUID();
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

void ComponentMesh::Save(JSON_Value* componentsJSON) {

	JSON_Value* componentJSON = componentsJSON->createValue();
	componentJSON->addInt("Type", type);
	componentJSON->addString("Path", path);

	componentsJSON->addValue("Mesh", componentJSON);
}

//void ComponentMesh::Load(JSON_Value* componentJSON) {
//
//	App->modelLoader->GenerateMesh(idMesh, meshComp, path);
//}