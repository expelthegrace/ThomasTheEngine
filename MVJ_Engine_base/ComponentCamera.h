#ifndef _COMPCAMERA_
#define _COMPCAMERA_

#include "Globals.h"
#include "Component.h"
#include "ModuleCamera.h"



class ComponentCamera :
	public Component
{
public:
	ComponentCamera();
	~ComponentCamera();

	void UpdateFrustum();
	update_status Update() override;


public:

	Frustum frustum;
	FBOset fboSet;

	int w, h;
	math::float4x4 view = float4x4::identity;
	math::float4x4 projection = float4x4::identity;

private:
	
	bool cameraChanged = true;
	
};

#endif