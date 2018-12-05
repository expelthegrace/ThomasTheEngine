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

	void NewGameObject(char* name);

	void NewGOSelected(GameObject* GO);

public:
	GameObject* ROOT;
	GameObject* GO_selected = nullptr;

private:

	//std::vector<GameObject*> gameObjects;

};

#endif