#ifndef _GAMEOBJ_
#define _GAMEOBJ_

#include "Globals.h"

class Component;

class GameObject
{
public:
	GameObject();
	~GameObject();

	Component* CreateComponent(type_comp type);

public:
	bool active;
	char* name;
	Component* components;
};

#endif

