#include "ComponentMaterial.h"
#include "Application.h"
ComponentMaterial::ComponentMaterial()
{
	UID = App->generateUID();
}

ComponentMaterial::ComponentMaterial(unsigned material)
{
	this->material = material;
}


ComponentMaterial::~ComponentMaterial()
{
}
