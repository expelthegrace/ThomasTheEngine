#ifndef _MODULESCENE_
#define _MODULESCENE_

#include "Module.h"
#include <vector>
#include <map>
#include "MathGeoLib.h"

class GameObject;
class Quadtree;
class ComponentCamera;
class ComponentLight;

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

	void DragInputManager(char * path);

	void DeleteSelected();
	void DuplicateSelected();

	void mouseClick(int mouseXi, int mouseYi);

public:
	GameObject* ROOT;
	GameObject* GO_selected = nullptr;
	Quadtree* quadTree;
	bool showQuad = true;
	ComponentCamera* mainCamera = nullptr;
	ComponentLight* mainLight = nullptr;
	char* scenePath = "JSON/sceneDefault.tte";
	bool drawDebugDraw = true;
	math::LineSegment ray = LineSegment(float3(-100, -100, -100), float3(-100, -100, -100));

private:
	std::map<unsigned, GameObject*> gameObjects;
	GameObject* closestToCam(GameObject* go1, GameObject* go2);
	

};

#endif