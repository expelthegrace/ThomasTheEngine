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
	unsigned texture;
	float diffuse_k = 0.5f;
	float specular_k = 0.5f;
	float shininess = 64.f;

};

#endif