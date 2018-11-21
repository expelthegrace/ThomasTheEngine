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
	update_status Update() override;
	GameObject* CreateModel(char* name, GameObject* parent, char * path);
	GameObject* FindByName(char* name);
	void MoveTo(GameObject* source, GameObject* newParent);

private:

	//std::vector<GameObject*> gameObjects;
	GameObject* ROOT;
};

#endif