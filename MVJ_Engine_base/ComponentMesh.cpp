#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "Application.h"
#include "JSONManager.h"
#include "ModuleRender.h"
#include "ModuleModelLoader.h"

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
	App->renderer->DeleteMesh(this->UID);
	delete[] mesh.vertices;
	delete[] mesh.indices;
	delete[] mesh.normals;
}

void ComponentMesh::Save(JSON_Value* componentsJSON) {

	JSON_Value* componentJSON = componentsJSON->createValue();
	componentJSON->addInt("Type", type);
	componentJSON->addString("Path", path.c_str());
	componentJSON->addInt("idMesh", idMesh);

	componentsJSON->addValue("Mesh", componentJSON);
}

void ComponentMesh::Load(JSON_Value* componentJSON) {
	path = componentJSON->getString("Path");
	idMesh = componentJSON->getInt("idMesh");

	//Load mesh
	App->modelLoader->GenerateMesh(my_go, idMesh, this, path.c_str());
	this->avaliable = true;
	App->renderer->meshComponents.push_back(this);
	
}