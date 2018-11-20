#include "GameObject.h"
#include "ModuleRender.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"


GameObject::GameObject(char * name, bool active, GameObject * parent)
{
	this->name = name;
	this->active = active;
	this->parent = parent;
}


GameObject::~GameObject()
{
	delete name;
	for (int i = 0; i < components.size(); ++i) delete components[i];
	components.clear();
}

GameObject* GameObject::AddChild(GameObject* child) {
	children.push_back(child);
	return child;
}

Component* GameObject::CreateComponent(type_comp type, int id, char * path) {
	Component* comp;

	

	return comp;
}


