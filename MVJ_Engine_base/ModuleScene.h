#ifndef _MODULESCENE_
#define _MODULESCENE_

#include "Module.h"


class GameObject;

class ModuleScene :
	public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Init();

public:

	GameObject * gameObjects;
};

#endif