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

public:
	float3 position, scale;
	Quat rotation;
};

#endif