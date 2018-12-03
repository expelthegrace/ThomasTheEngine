#ifndef _COMPMATERIAL_
#define _COMPMATERIAL_

#include "Component.h"
class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(unsigned material);
	~ComponentMaterial();



public:
	unsigned material;
};

#endif