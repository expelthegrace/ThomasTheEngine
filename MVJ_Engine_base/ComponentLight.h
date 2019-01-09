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
	float3 position, initialPosition    = { 0, 1, 0 };
	float3 direction, initialDirection  = { 0, 0, 1 };
	float3 colorLight = { 0.5f, 0.5f, 0.5f };
	float intensity = 0.8f;

};

#endif