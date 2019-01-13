#include "ComponentLight.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleScene.h"
#include "Application.h"
#include "debugdraw.h"
#include "JSONManager.h"

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
	colorLight_Intensity = float3(colorLight.x * intensity, colorLight.y * intensity, colorLight.z * intensity);

	const ddVec3 boxColor = { 0.f, 0.6f, 0.8f };
	dd::sphere(position, boxColor, 0.4 * App->GameScale);

	return UPDATE_CONTINUE;
}

void ComponentLight::Reset() {
	colorLight = { 0.5f, 0.5f, 0.5f };
	intensity = 1.f;
}

void ComponentLight::Save(JSON_Value* componentsJSON) {
	JSON_Value* componentJSON = componentsJSON->createValue();
	componentJSON->addInt("Type", type);
	componentJSON->addVector3("colorLight", colorLight);
	componentJSON->addFloat("Intensity", intensity);

	componentsJSON->addValue("Light", componentJSON);
}

void ComponentLight::Load(JSON_Value* componentJSON) {
	colorLight = componentJSON->getVector3("colorLight");
	intensity  = componentJSON->getFloat("Intensity");

}
