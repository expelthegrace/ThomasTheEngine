#ifndef _COMPLIGHT_
#define _COMPLIGHT_

#include "Component.h"
#include "MathGeoLib.h"

class GameObject;
class JSON_Value;

class ComponentLight :
	public Component
{
public:
	ComponentLight(GameObject* my_go);
	~ComponentLight();

	update_status Update() override;

	void Save(JSON_Value* componentsJSON) override;
	void Load(JSON_Value* componentJSON) override;

	void Reset();

public:
	float3 position  = { 0, 1, 0 };
	//float3 direction  = { 0, 0, 1 };
	float3 colorLight = { 0.5f, 0.5f, 0.5f };
	float intensity = 1.f;

	float3 colorLight_Intensity = colorLight;
	

private:
	

};

#endif