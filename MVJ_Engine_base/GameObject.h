#ifndef _GAMEOBJ_
#define _GAMEOBJ_

#include "Globals.h"
#include <vector>

class Component;

class GameObject
{
public:
	GameObject(char * name, bool active, GameObject * parent);
	~GameObject();

	Component* CreateComponent(type_comp type, int id, char * path);
	GameObject* AddChild(GameObject * child);
	// delete gameobject (recursiu)

public:
	bool active;
	char* name;
	GameObject* parent;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
};

#endif

