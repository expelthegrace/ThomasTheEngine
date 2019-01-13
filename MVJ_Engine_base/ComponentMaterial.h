#ifndef _COMPMATERIAL_
#define _COMPMATERIAL_

#include "Component.h"
#include <string>

class GameObject;
class JSON_Value;

class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial(GameObject * my_go);
	ComponentMaterial(GameObject * my_go, unsigned material);
	~ComponentMaterial();

	void Save(JSON_Value* componentsJSON) override;
	void Load(JSON_Value* componentJSON) override;


public:
	unsigned texture;
	float diffuse_k = 1.f;
	float specular_k = 0.5f;
	float shininess = 64.f;

	std::string path = "";
	/**  idMaterial indexes the mesh that is vinculated with */
	unsigned idMaterial = 0;

	bool hasTexture = false;

};

#endif