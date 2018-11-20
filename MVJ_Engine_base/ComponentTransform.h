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

public:
	float3 position, scale, rotation;
	
};

#endif