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
	void ParentChanged(GameObject* lastParent);
	void Save(JSON_Value* componentsJSON) override;

public:
	float3 position, scale, eulerRot, globalPosition;
	math::Quat rotation;
	float4x4 model;
	GameObject* my_go;

	bool changed = true;
	
};

#endif