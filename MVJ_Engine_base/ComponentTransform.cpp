#include "ComponentTransform.h"
#include "ComponentBB.h"
#include "Application.h"

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


void ComponentTransform::UpdateTransform(bool updateChilds) {

	if (changed || updateChilds) {

		model.Set(float4x4::FromTRS(position, rotation, scale));		
		if (my_go->parent != nullptr) model = my_go->parent->transform->model * model;

		globalPosition = (model * float4(position.x, position.y, position.z, 1.f)).xyz();

		my_go->BB->UpdateBB();

		for (int i = 0; i < my_go->children.size(); ++i) my_go->children[i]->transform->UpdateTransform(true);
		changed = false;
	}
	else for (int i = 0; i < my_go->children.size(); ++i) my_go->children[i]->transform->UpdateTransform(false);
		
}