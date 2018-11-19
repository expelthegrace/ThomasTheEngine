#pragma once
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

