#pragma once
#include "Module.h"
#include "MathGeoLib.h"
#include "ModuleInput.h"

class ModuleCamera :
	public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool            Init();
	update_status   Update();
	bool            CleanUp();

	void UpdateFrustum();

public:
	Punt lastMouse, actualMouse;

	math::float3 camPos;
	float distCamVrp;
	float movementSpeed;
	float speed1;
	float speed2;
	float rotationSpeed;

	void LookAt(math::float3& target);
	void FocusModel();
	void ResizeFOV(unsigned & newWidth, unsigned & newHeight);

	int screenWidth, screenHeight;

	math::float3 up;
	math::float3 fwd;
	math::float3 side;

	math::float3 Xaxis;
	math::float3 Yaxis;
	math::float3 Zaxis;

	Frustum frustum;
	math::float4x4 view;
	math::float4x4 projection;

	bool pressingRightMouse;

private:
	bool cameraChanged;

};

