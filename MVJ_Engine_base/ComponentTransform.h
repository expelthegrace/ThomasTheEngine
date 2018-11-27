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
	void UpdateTransform(bool updateChilds);

public:
	float3 position, scale;
	math::Quat rotation;
	float4x4 model;
	GameObject* my_go;

	bool changed = false;
	
};

#endif