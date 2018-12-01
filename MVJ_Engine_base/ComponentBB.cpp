#include "ComponentBB.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ModuleRender.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "debugdraw.h"
#include "ModuleScene.h"


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
	//Aabb = new math::AABB();
	Aabb->SetFrom(total, totalPoints);

	Aabb->GetCornerPoints(cornersAABB);

}

update_status ComponentBB::Update() {

	if (Aabb->IsFinite() && my_go->active) {
		const ddVec3 boxColor = { 0.0f, 0.8f, 0.8f };
		dd::aabb(Aabb->minPoint, Aabb->maxPoint, boxColor);
	}
	return UPDATE_CONTINUE;
}

void ComponentBB::UpdateBB() {
	
	if (Aabb->IsFinite() && my_go->active) {
		std::vector<Component*> comps = my_go->GetComponents(MESH);
		SetAABB((std::vector<ComponentMesh*>*) &comps);
		Aabb->TransformAsAABB(my_go->transform->model);		
	}
	
	for (int i = 0; i < my_go->children.size(); ++i) my_go->children[i]->BB->UpdateBB();
}

ComponentBB::~ComponentBB()
{
	delete cornersAABB;
	delete Aabb;
}
