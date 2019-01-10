#ifndef _COMPLIGHT_
#define _COMPLIGHT_

#include "Component.h"
#include "MathGeoLib.h"

class GameObject;

class ComponentLight :
	public Component
{
public:
	ComponentLight(GameObject* my_go);
	~ComponentLight();

	update_status Update() override;

public:
	float3 position  = { 0, 1, 0 };
	//float3 direction  = { 0, 0, 1 };
	float3 colorLight = { 0.5f, 0.5f, 0.5f };
	float3 colorLight_Intensity = colorLight;
	float intensity = 0.8f;

private:
	

};

#endif