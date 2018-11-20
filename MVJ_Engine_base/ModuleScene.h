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
	GameObject* CreateModel(char * path);

private:

	GameObject * gameObjects;
};

#endif