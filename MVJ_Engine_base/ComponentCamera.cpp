#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "JSONManager.h"

ComponentCamera::ComponentCamera(GameObject* my_go)
{
	this->my_go = my_go;
	type = CAMERA;
	UID = App->generateUID();
	fboSet.fbo = 0;
	fboSet.fb_depth = 0;
	fboSet.fb_tex = 0;
	fboSet.fb_width = 0;
	fboSet.fb_height = 0;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = my_go->transform->globalPosition;
	frustum.front = math::float3(0, 0, -1);
	frustum.up = math::float3(0, 1, 0);
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	
	w = App->camera->editorWidth;
	h = App->camera->editorHeight;

	float aspect = (float)w / (float)h;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);

	App->renderer->cameras.push_back(this);

	if (App->scene->mainCamera == nullptr) App->scene->mainCamera = this;
}

update_status ComponentCamera::Update() {


	if (cameraChanged) {
		UpdateFrustum();
		cameraChanged = false;
	}
	return UPDATE_CONTINUE;
}

void ComponentCamera::UpdateFrustum() {

	frustum.pos = my_go->transform->globalPosition;

	w = App->camera->editorWidth;
	h = App->camera->editorHeight;
	float aspect = (float)w / (float)h;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);

	view = frustum.ViewMatrix();
	projection = frustum.ProjectionMatrix();
	
}

ComponentCamera::~ComponentCamera()
{
	App->renderer->RemoveCamera(this);
}

void ComponentCamera::Save(JSON_Value* componentsJSON) {

	JSON_Value* componentJSON = componentsJSON->createValue();
	componentJSON->addInt("Type", type);
	
	componentsJSON->addValue("Camera", componentJSON);

}