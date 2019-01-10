#include "ComponentLight.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleScene.h"
#include "Application.h"
#include "debugdraw.h"


ComponentLight::ComponentLight(GameObject * my_go)
{
	this->my_go = my_go;
	this->type = LIGHT;
	if (App->scene->mainLight == nullptr) App->scene->mainLight = this;
}


ComponentLight::~ComponentLight()
{
	my_go = nullptr;
}

update_status ComponentLight::Update() {
	position = my_go->transform->globalPosition;
	
	//direction = (my_go->transform->rotation * initialDirection).Normalized();
	colorLight_Intensity = float3(min(colorLight.x * intensity, 1.f), min(colorLight.y * intensity, 1.f), min(colorLight.z * intensity, 1.f));

	const ddVec3 boxColor = { 0.f, 0.6f, 0.8f };
	dd::sphere(position, boxColor, 0.7 * App->GameScale);

	return UPDATE_CONTINUE;
}

