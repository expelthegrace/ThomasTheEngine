#include "ComponentTransform.h"


ComponentTransform::ComponentTransform()
{
	scale = { 1.f,1.f,1.f };
	position = { 0.f,0.f,0.f };
	rotation = { 0.f, 0.f, 0.f };
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Reset() {
	scale = { 1.f,1.f,1.f };
	position = { 0.f,0.f,0.f };
	rotation = { 0.f, 0.f, 0.f };
}

update_status ComponentTransform::Update() {

	float4x4 newpos = math::TranslateOp(position).ToFloat4x4();
	Quat newrotation = Quat::FromEulerXYZ(rotation.x,rotation.y, rotation.z);
	float4x4 newscale = math::ScaleOp(scale).ToFloat4x4();
	
	model = newpos * newrotation * newscale;
	if (my_go->parent != nullptr) model = my_go->parent->transform->model * model;

	for (int i = 0; i < my_go->children.size(); ++i) my_go->children[i]->transform->Update();

	return UPDATE_CONTINUE;
}