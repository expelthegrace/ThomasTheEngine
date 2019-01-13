#include "ComponentMaterial.h"
#include "Application.h"
#include "JSONManager.h"
#include "ModuleModelLoader.h"
#include "GameObject.h"

ComponentMaterial::ComponentMaterial(GameObject * my_go)
{
	this->type = MATERIAL;
	this->my_go = my_go;
	UID = App->generateUID();
}

ComponentMaterial::ComponentMaterial(GameObject * my_go, unsigned texture)
{
	this->type = MATERIAL;
	this->my_go = my_go;
	this->texture = texture;
}


ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Save(JSON_Value* componentsJSON) {
	JSON_Value* componentJSON = componentsJSON->createValue();
	componentJSON->addInt("Type", type);
	componentJSON->addString("Path", path.c_str());
	componentJSON->addInt("idMaterial", idMaterial);
	componentJSON->addFloat("diffuse_k", diffuse_k);
	componentJSON->addFloat("specular_k", specular_k);
	componentJSON->addFloat("shininess", shininess);


	componentsJSON->addValue("Material", componentJSON);
}

void ComponentMaterial::Load(JSON_Value* componentJSON) {
	path = componentJSON->getString("Path");
	idMaterial = componentJSON->getInt("idMaterial");
	texture = App->modelLoader->GenerateMaterial(idMaterial, path.c_str());
	if (texture != -1) hasTexture = true;
	diffuse_k = componentJSON->getFloat("diffuse_k");
	specular_k = componentJSON->getFloat("specular_k");
	shininess = componentJSON->getFloat("shininess");


}
