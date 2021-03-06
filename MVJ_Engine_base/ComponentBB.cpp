#include "ComponentBB.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ModuleRender.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "debugdraw.h"
#include "ModuleScene.h"
#include "JSONManager.h"


ComponentBB::ComponentBB(GameObject* my_go)
{
	/*
	float3* unitaryCube = new float3[8]{
	float3(-0.5f,  -0.5f, -0.5f), 
	float3( 0.5f,  -0.5f, -0.5f),
	float3( 0.5f,   0.5f, -0.5f),
	float3(-0.5f,   0.5f, -0.5f),
	float3(-0.5f,  -0.5f,  0.5f),
	float3( 0.5f,  -0.5f,  0.5f),
	float3( 0.5f,   0.5f,  0.5f),
	float3(-0.5f,   0.5f,  0.5f)
	};

	Aabb = new math::AABB();

	Aabb->SetFrom(unitaryCube, 8);
	Aabb->GetCornerPoints(cornersAABB);
	*/
	UID = App->generateUID();
	type = BB;
	cornersAABB = new float3[8];
	Aabb = new math::AABB();
	Aabb->SetNegativeInfinity();

	this->my_go = my_go;
}

void ComponentBB::SetAABB(std::vector<ComponentMesh*>* meshes)
{
	Aabb->SetNegativeInfinity();

	std::vector<ComponentMesh*> meshesActual = *meshes;

	int totalPoints = 0;
	for (int i = 0; i < meshesActual.size(); ++i) totalPoints += meshesActual[i]->mesh.numVertices;
	float3* total = new float3[totalPoints];

	int index = 0;
	for (int i = 0; i < meshesActual.size(); ++i) {
		for (int j = 0; j < meshesActual[i]->mesh.numVertices; ++j) {
			total[index] = meshesActual[i]->mesh.vertices[j];
			++index;
		}
	}
	Aabb->SetFrom(total, totalPoints);
	Aabb->GetCornerPoints(cornersAABB);

	initialMin = Aabb->minPoint;
	initialMax = Aabb->maxPoint;

	delete total;
}

void ComponentBB::SetAABB(ComponentMesh* meshComp)
{
	Aabb->SetNegativeInfinity();

	//for (int i = 0; i < meshesActual.size(); ++i) totalPoints += meshesActual[i]->mesh.numVertices;
	float3* total = new float3[meshComp->mesh.numVertices];

	for (int i = 0; i  <meshComp->mesh.numVertices; ++i) 
			total[i] = meshComp->mesh.vertices[i];
		
	Aabb->SetFrom(total, meshComp->mesh.numVertices);
	Aabb->GetCornerPoints(cornersAABB);

	initialMin = Aabb->minPoint;
	initialMax = Aabb->maxPoint;

	delete total;
}

update_status ComponentBB::Update() {

	if (my_go->selected && Aabb->IsFinite() && my_go->active) {
		const ddVec3 boxColor = { 0.0f, 0.8f, 0.8f };
		dd::aabb(Aabb->minPoint, Aabb->maxPoint, boxColor);
	}
	return UPDATE_CONTINUE;
}

void ComponentBB::UpdateBB() {
	
	if (Aabb->IsFinite() && my_go->active) {
		//std::vector<Component*> comps = my_go->GetComponents(MESH);
		//SetAABB((std::vector<ComponentMesh*>*) &comps);
		delete Aabb;
		Aabb = new math::AABB(initialMin, initialMax);
		Aabb->TransformAsAABB(my_go->transform->model);		
	}
	
	for (int i = 0; i < my_go->children.size(); ++i) my_go->children[i]->BB->UpdateBB();
}

ComponentBB::~ComponentBB()
{
	delete cornersAABB;
	delete Aabb;
}

void ComponentBB::Save(JSON_Value* componentsJSON) {

	JSON_Value* componentJSON = componentsJSON->createValue();
	componentJSON->addInt("Type", type);
	componentJSON->addBool("isFinite", Aabb->IsFinite());
	componentJSON->addUint("UID", UID);
	componentJSON->addVector3("minPoint", initialMin);
	componentJSON->addVector3("maxPoint", initialMax);

	componentsJSON->addValue("BB", componentJSON);
}

void ComponentBB::Load(JSON_Value* componentJSON) {

	UID = componentJSON->getUint("UID");

	if (componentJSON->getBool("isFinite")) {
		delete Aabb;
		initialMin = componentJSON->getVector3("minPoint");
		initialMax = componentJSON->getVector3("maxPoint");
		Aabb = new AABB(initialMin, initialMax);
	}
	
}
