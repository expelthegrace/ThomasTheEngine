#ifndef _MODULESCENE_
#define _MODULESCENE_

#include "Module.h"
#include <vector>
#include <map>

class GameObject;
class Quadtree;
class ComponentCamera;

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

	GameObject* getGOByID(unsigned uid);
	void NewGameObject(char* name);

	void NewGOSelected(GameObject* GO);

	void SaveScene();
	void LoadScene();

	void ClearScene();

	void DeleteSelected();
	void DuplicateSelected();

public:
	GameObject* ROOT;
	GameObject* GO_selected = nullptr;
	Quadtree* quadTree;
	bool showQuad = true;
	ComponentCamera* mainCamera = nullptr;
	char* scenePath = "sceneDefault.tte";

private:
	std::map<unsigned, GameObject*> gameObjects;

	

};

#endif