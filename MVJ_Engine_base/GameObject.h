#ifndef _GAMEOBJ_
#define _GAMEOBJ_

#include "Globals.h"
#include <vector>


class Component;
class ComponentTransform;
class ComponentBB;
class GameObject
{
public:

	GameObject(char * name, bool active, GameObject * parent);
	~GameObject();

	update_status Update();

	Component* CreateComponent(type_comp type, int id, char * path);

	//void CreateSphere(float radius, int subdivisions);

	GameObject* AddChild(GameObject * child);
	void AddComponent(Component * comp);
	int GetChildIndexByUID(unsigned uid);

	void MoveToNewParent(GameObject* newParent);

	std::vector<Component*> GetComponents(type_comp type);
	// delete gameobject (recursiu)
	void SetActive(bool active);

	void SelectGO(bool selected);
	void SelectGOChild(bool selected);

public:
	unsigned UID;
	bool selected = false;
	bool child_selected = false;
	bool active;
	char* name;
	GameObject* parent;
	ComponentTransform* transform;
	ComponentBB* BB;
	std::vector<GameObject*> children;
	std::vector<Component*> components;

private:
	bool lastFrameActive;

};

#endif

