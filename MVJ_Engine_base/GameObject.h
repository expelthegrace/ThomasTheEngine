#ifndef _GAMEOBJ_
#define _GAMEOBJ_

#include "Globals.h"
#include <vector>


class Component;
class ComponentTransform;
class ComponentBB;
class ComponentMesh;
class ComponentMaterial;
struct JSON_Value;
class GameObject
{
public:
	GameObject();
	GameObject(char * name, bool active, GameObject * parent);
	~GameObject();

	update_status Update();

	Component* CreateComponent(type_comp type);

	//void CreateSphere(float radius, int subdivisions);

	GameObject* AddChild(GameObject * child);
	void AddComponent(Component * comp);
	int GetChildIndexByUID(unsigned uid);

	void MoveToNewParent(GameObject* newParent);

	std::vector<Component*> GetComponents(type_comp type);
	// delete gameobject (recursiu)
	void SetActive(bool active);

	void RemoveFromParent();

	void SelectGO(bool selected);
	void SelectGOChild(bool selected);
	void Save(JSON_Value* gameObjectsJSON);
	uint Load(JSON_Value* gameObjectJSON);


public:
	unsigned UID = 0;
	unsigned parentUID = 0;
	bool selected = false;
	bool child_selected = false;
	bool active;
	char* name;
	GameObject* parent = nullptr;
	ComponentTransform* transform = nullptr;
	ComponentBB* BB = nullptr;
	ComponentMesh* mesh = nullptr;
	ComponentMaterial* material = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;

private:
	bool lastFrameActive;

};

#endif

