#include "ComponentBB.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ModuleRender.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "debugdraw.h"

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

	cornersAABB = new float3[8];

	type = BB;
	Aabb = new math::AABB();

	Aabb->SetFrom(unitaryCube, 8);
	Aabb->GetCornerPoints(cornersAABB);
	*/

	cornersAABB = new float3[8];
	Aabb = new math::AABB();
	Aabb->SetNegativeInfinity();

	this->my_go = my_go;
}

void ComponentBB::SetAABB(std::vector<ComponentMesh*>* meshes)
{
	delete Aabb;

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
	Aabb = new math::AABB();
	Aabb->SetFrom(total, totalPoints);

	Aabb->GetCornerPoints(cornersAABB);

	char* b = new char[50];
	sprintf(b, "GameObject AABB corners:\n");
	App->menu->console.AddLog(b);

	for (int i = 0; i < 8; ++i) {
		sprintf(b, "%f, %f, %f \n", cornersAABB[i].x, cornersAABB[i].y, cornersAABB[i].z);
		App->menu->console.AddLog(b);
	}
}

update_status ComponentBB::Update() {
	
	//const float3 center = Aabb->CenterPoint();

	//Aabb->Scale(&center, my_go->transform->scale);
	
	
	if (Aabb->IsFinite() && my_go->active) {
		const ddVec3 boxColor = { 0.0f, 0.8f, 0.8f };
		dd::aabb(Aabb->minPoint, Aabb->maxPoint, boxColor);
	}
	
	
	return UPDATE_CONTINUE;

}

ComponentBB::~ComponentBB()
{
	delete cornersAABB;
	delete Aabb;
}
