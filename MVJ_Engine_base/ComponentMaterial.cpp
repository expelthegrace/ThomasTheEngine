#include "ComponentMaterial.h"
#include "Application.h"
ComponentMaterial::ComponentMaterial()
{
	UID = App->generateUID();
}

ComponentMaterial::ComponentMaterial(unsigned texture)
{
	this->texture = texture;
}


ComponentMaterial::~ComponentMaterial()
{
}


