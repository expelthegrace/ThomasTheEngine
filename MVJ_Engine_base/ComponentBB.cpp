#include "ComponentBB.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ModuleRender.h"
#include "Application.h"
#include "ModuleMenu.h"

ComponentBB::ComponentBB(GameObject* my_go)
{
	float3* unitaryCube = new float3[8]{
	float3(-0.5f, -0.5f, -0.5f), 
	float3(0.5f, -0.5f, -0.5f),
	float3(0.5f,  0.5f, -0.5f),
	float3(-0.5f,  0.5f, -0.5f),
	float3(-0.5f, -0.5f,  0.5f),
	float3(0.5f, -0.5f,  0.5f),
	float3(0.5f,  0.5,  0.5f),
	float3(-0.5f,  0.5f,  0.5f),
	};

	type = BB;
	Aabb = new math::AABB();
	Aabb->SetFrom(unitaryCube, 24);

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

	float3* corners = new float3[8];
	Aabb->GetCornerPoints(corners);

	char* b = new char[50];
	sprintf(b, "GameObject AABB corners:\n");
	App->menu->console.AddLog(b);

	for (int i = 0; i < 8; ++i) {
		sprintf(b, "%f, %f, %f \n", corners[i].x, corners[i].y, corners[i].z);
		App->menu->console.AddLog(b);
	}
}

update_status ComponentBB::Update() {
	
	//const float3 center = Aabb->CenterPoint();

	//Aabb->Scale(&center, my_go->transform->scale);
	//Aabb->Translate(my_go->transform->position);
	App->renderer->RenderBB(this);

	
	return UPDATE_CONTINUE;

}

ComponentBB::~ComponentBB()
{
	delete Aabb;
}
