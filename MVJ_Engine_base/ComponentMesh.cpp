#include "ComponentMesh.h"
#include "GameObject.h"
#include "Application.h"
#include "JSONManager.h"
#include "ModuleRender.h"

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
	delete mesh.vertices;
}

void ComponentMesh::Save(JSON_Value* componentsJSON) {

	JSON_Value* componentJSON = componentsJSON->createValue();
	componentJSON->addInt("Type", type);
	componentJSON->addString("Path", path);
	componentJSON->addInt("idMesh", idMesh);



	componentsJSON->addValue("Mesh", componentJSON);
}

void ComponentMesh::Load(JSON_Value* componentJSON) {
	//path = componentJSON->getString("Path");

	std::string strAux = componentJSON->getString("Path");
	path = new char[strAux.length()];//&strAux[0u];
	idMesh = componentJSON->getInt("idMesh");

	//App->modelLoader->GenerateMesh(idMesh, meshComp, path);
}