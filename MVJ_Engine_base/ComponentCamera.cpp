#include "ComponentCamera.h"
#include "Application.h"


ComponentCamera::ComponentCamera()
{
	UID = App->generateUID();
	fboSet.fbo = 0;
	fboSet.fb_depth = 0;
	fboSet.fb_tex = 0;
	fboSet.fb_width = 0;
	fboSet.fb_height = 0;
}

update_status ComponentCamera::Update() {




	if (cameraChanged) UpdateFrustum();

	return UPDATE_CONTINUE;
}

void ComponentCamera::UpdateFrustum() {

	view = frustum.ViewMatrix();
	projection = frustum.ProjectionMatrix();
	
}

ComponentCamera::~ComponentCamera()
{
}
