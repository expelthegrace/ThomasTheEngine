#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "JSONManager.h"
#include "debugdraw.h"

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
	frustum.front = initialFront;
	frustum.up = initialUp;
	frustum.nearPlaneDistance = 0.1f * App->GameScale;
	frustum.farPlaneDistance = 100.0f * App->GameScale;
	frustum.verticalFov = math::pi / 4.0f;
	
	w = App->camera->editorWidth;
	h = App->camera->editorHeight;

	float aspect = (float)w / (float)h;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);

	App->renderer->cameras.push_back(this);

	if (App->scene->mainCamera == nullptr) App->scene->mainCamera = this;
}

void ComponentCamera::Reset() {
	frustum.nearPlaneDistance = 0.1f * App->GameScale;
	frustum.farPlaneDistance = 100.0f * App->GameScale;

	UpdateFrustum();

}

update_status ComponentCamera::Update() {


	if (cameraChanged) {
		UpdateFrustum();
		cameraChanged = false;
	}
	
	const ddVec3 boxColor = { 0.2f, 0.8f, 0.8f };
	dd::frustum((projection * view).Inverted(), boxColor);
	
	
	return UPDATE_CONTINUE;
}

void ComponentCamera::UpdateFrustum() {

	frustum.pos = my_go->transform->globalPosition;

	frustum.front = (my_go->transform->rotation * initialFront).Normalized();
	frustum.up = (my_go->transform->rotation * initialUp).Normalized();

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
	componentJSON->addFloat("NearPlane", frustum.nearPlaneDistance);
	componentJSON->addFloat("FarPlane",  frustum.farPlaneDistance);
	
	componentsJSON->addValue("Camera", componentJSON);
}

void ComponentCamera::Load(JSON_Value* componentJSON) {

	frustum.nearPlaneDistance = componentJSON->getFloat("NearPlane");
	frustum.farPlaneDistance = componentJSON->getFloat("FarPlane");

}