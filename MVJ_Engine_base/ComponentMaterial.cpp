#include "ComponentMaterial.h"
#include "Application.h"
#include "JSONManager.h"
#include "ModuleModelLoader.h"
#include "GameObject.h"
#include <string>
using namespace std;

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

int ComponentMaterial::LoadTexture(const char * pathText) {
	string pathString = pathText;

	for (int i = 0; i < pathString.length(); i++)
	{
		if (pathString[i] == '\\')
			pathString[i] = '/';
	}

	uint pos_slash = pathString.find_last_of('/');
	uint pos_dot = pathString.find_last_of('.');

	string extension = pathString.substr(pos_dot + 1);
	string name = pathString.substr(pos_slash + 1, pos_dot - pos_slash - 1);

	textureDiff = App->modelLoader->LoadTexture(pathText);
	if (textureDiff != -1) hasTexture = true;
	pathDiffuse = "./Assets/" + name + "." + extension;

	return textureDiff;
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
	LoadTexture(pathDiffuse.c_str());

	diffuse_k = componentJSON->getFloat("diffuse_k");
	specular_k = componentJSON->getFloat("specular_k");
	shininess = componentJSON->getFloat("shininess");

}
