#ifndef _MODULESCENE_
#define _MODULESCENE_

#include "Module.h"
#include <vector>


class GameObject;

class ModuleScene :
	public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Init();
	GameObject* CreateModel(char* name, GameObject* parent, char * path);

private:

	std::vector<GameObject*> gameObjects;
	GameObject* ROOT;
};

#endif