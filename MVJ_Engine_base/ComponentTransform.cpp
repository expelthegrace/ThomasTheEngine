#include "ComponentTransform.h"


ComponentTransform::ComponentTransform()
{
	scale = { 1.f,1.f,1.f };
	position = { 0.f,0.f,0.f };
	rotation = Quat::FromEulerXYZ(0.f, 0.f, 0.f );
	type = TRANSFORM;

	model.Set(float4x4::FromTRS(position, rotation, scale));
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Reset() {
	scale = { 1.f,1.f,1.f };
	position = { 0.f,0.f,0.f };
	rotation = Quat::FromEulerXYZ(0.f, 0.f, 0.f);
	changed = true;

}

void ComponentTransform::UpdateTransform(bool updateChilds) {

	if (changed || updateChilds) {
		model.Set(float4x4::FromTRS(position, rotation, scale));
		if (my_go->parent != nullptr) model = my_go->parent->transform->model * model;
		for (int i = 0; i < my_go->children.size(); ++i) my_go->children[i]->transform->UpdateTransform(true);
		changed = false;
	}
	else for (int i = 0; i < my_go->children.size(); ++i) my_go->children[i]->transform->UpdateTransform(false);
		
}