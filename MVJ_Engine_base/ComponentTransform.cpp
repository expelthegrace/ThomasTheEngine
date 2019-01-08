#include "ComponentTransform.h"
#include "ComponentBB.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Quadtree.h"
#include "ComponentCamera.h"
#include "JSONManager.h"

ComponentTransform::ComponentTransform()
{
	UID = App->generateUID();
	scale = { 1.f,1.f,1.f };
	eulerRot = position = globalPosition = { 0.f,0.f,0.f };
	rotation = Quat::FromEulerXYZ(0.f, 0.f, 0.f );
	type = TRANSFORM;

	model.Set(float4x4::FromTRS(position, rotation, scale));
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Reset() {
	scale = { 1.f,1.f,1.f };
	eulerRot = position = { 0.f,0.f,0.f };
	rotation = Quat::FromEulerXYZ(0.f, 0.f, 0.f);
	changed = true;

	//for (int i = 0; i < my_go->children.size(); ++i) my_go->children[i]->transform->Reset();

}

void ComponentTransform::ParentChanged(GameObject* lastParent) {

	model.Set(float4x4::FromTRS(position, rotation, scale));
	model = lastParent->transform->model * model; // <- child of someone to global space

	model = my_go->parent->transform->model.Inverted() * model;
	model.Decompose(position, rotation, scale);
	eulerRot = rotation.ToEulerXYZ();
	changed = true;
}

void ComponentTransform::UpdateTransform(bool updateChilds) {

	if (changed || updateChilds) {

		Quadtree* lastQT = App->scene->quadTree->Find(this->my_go);

		model.Set(float4x4::FromTRS(position, rotation, scale));		
		if (my_go->parent != nullptr) {
			model = my_go->parent->transform->model * model;
			globalPosition = (my_go->parent->transform->model * float4(position.x, position.y, position.z, 1.f)).xyz();
		}
		else globalPosition = position;

		// Update BB
		my_go->BB->UpdateBB();

		//Update Camera components
		std::vector<Component*> comps = my_go->GetComponents(CAMERA);
		for (int i = 0; i < comps.size(); ++i) ((ComponentCamera*)comps[i])->UpdateFrustum();

		//Update children
		for (int i = 0; i < my_go->children.size(); ++i) my_go->children[i]->transform->UpdateTransform(true);

		//Update QuadTree
		App->scene->quadTree->MoveGO(this->my_go, lastQT);

		lastQT = nullptr;
		changed = false;
	}
	else for (int i = 0; i < my_go->children.size(); ++i) my_go->children[i]->transform->UpdateTransform(false);
		
}

void ComponentTransform::Save(JSON_Value * componentsJSON) {

	JSON_Value* componentJSON = componentsJSON->createValue();
	componentJSON->addInt("Type", type);
	componentJSON->addVector3("Position", position);
	componentJSON->addQuat("Rotation", rotation);
	componentJSON->addVector3("Scale", scale);
	componentJSON->addTransform("Transform", model);
	
	componentsJSON->addValue("Transform", componentJSON);
}

void ComponentTransform::Load(JSON_Value * componentJSON) {

	position = componentJSON->getVector3("Position");
	rotation = componentJSON->getQuat("Rotation");
	scale = componentJSON->getVector3("Scale");
	changed = true;

}