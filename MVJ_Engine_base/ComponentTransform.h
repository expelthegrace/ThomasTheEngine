#ifndef _COMPTRANSFORM_
#define _COMPTRANSFORM_

#include "Component.h"
#include "MathGeoLib.h"

class ComponentTransform :
	public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	void Reset();
	update_status Update() override;

public:
	float3 position, scale, rotation;
	float4x4 model;
	GameObject* my_go;
	
};

#endif