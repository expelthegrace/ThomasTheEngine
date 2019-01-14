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
	this->textureDiff = texture;
}

void ComponentMaterial::LoadTexture(char * pathText) {
	textureDiff = App->modelLoader->LoadTexture(pathText);
	if (textureDiff != -1) hasTexture = true;
	pathDiffuse = pathText;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Save(JSON_Value* componentsJSON) {
	JSON_Value* componentJSON = componentsJSON->createValue();
	componentJSON->addInt("Type", type);
	componentJSON->addString("PathDiffuse", pathDiffuse.c_str());
	componentJSON->addInt("idMaterial", idMaterial);
	componentJSON->addFloat("diffuse_k", diffuse_k);
	componentJSON->addFloat("specular_k", specular_k);
	componentJSON->addFloat("shininess", shininess);


	componentsJSON->addValue("Material", componentJSON);
}

void ComponentMaterial::Load(JSON_Value* componentJSON) {
	pathDiffuse = componentJSON->getString("PathDiffuse");
	idMaterial = componentJSON->getInt("idMaterial");
	textureDiff = App->modelLoader->LoadTexture(pathDiffuse.c_str());
	if (textureDiff != -1) hasTexture = true;
	diffuse_k = componentJSON->getFloat("diffuse_k");
	specular_k = componentJSON->getFloat("specular_k");
	shininess = componentJSON->getFloat("shininess");


}
