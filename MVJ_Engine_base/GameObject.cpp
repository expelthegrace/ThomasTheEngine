#include "GameObject.h"
#include "ModuleRender.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentBB.h"
#include "JSONManager.h"
#include "ModuleScene.h"
#include "Quadtree.h"


GameObject::GameObject() {
	
}

GameObject::GameObject(char * name, bool active, GameObject * parent)
{
	UID = App->generateUID();

	this->name = name;
	this->active = active;
	this->parent = parent;
	if (parent != nullptr) this->parentUID = parent->UID;
	this->lastFrameActive = this->active;

	if (parent != nullptr) parent->AddChild(this);

	BB = new ComponentBB(this);
	this->BB = BB;
	components.push_back(BB);

	transform = new ComponentTransform();
	transform->my_go = this;
	components.push_back(transform);
}


GameObject::~GameObject()
{
	//delete name;
	for (int i = 0; i < components.size(); ++i) delete components[i];
	for (int i = 0; i < children.size(); ++i) delete children[i];

	//App->scene->quadTree->Remove(this);

	components.clear();
	children.clear();
}

void GameObject::SetActive(bool active) {
	this->active = active;
	lastFrameActive = active;
	for (int i = 0; i < children.size(); ++i) children[i]->SetActive(active);
}

void GameObject::SelectGO(bool selected) {
	this->selected = selected;

	if (parent != nullptr) parent->SelectGOChild(selected);
}

void GameObject::SelectGOChild(bool selected) {
	this->child_selected = selected;
	if (parent != nullptr) parent->SelectGOChild(selected);
}

GameObject* GameObject::AddChild(GameObject* child) {
	children.push_back(child);
	return child;
}

void GameObject::AddComponent(Component* comp) {
	components.push_back(comp);
	
}

std::vector<Component*> GameObject::GetComponents(type_comp type) {
	std::vector<Component*> ret;
	for (int i = 0; i < components.size(); ++i) if (components[i]->type == type) ret.push_back(components[i]);
	return ret;
}

update_status GameObject::Update() {

	if (lastFrameActive != active) SetActive(active);

	for (int i = 0; i < components.size(); ++i) components[i]->Update();
	for (int i = 0; i < children.size(); ++i) children[i]->Update();

	return UPDATE_CONTINUE;
}

Component* GameObject::CreateComponent(type_comp type) {
	Component* ret = nullptr;
	bool itsNew = false;

	switch (type)
	{
	case TRANSFORM:
		
		transform = new ComponentTransform();
		itsNew = true;
		
		ret = transform;
		break;
	case MESH:
		
		ret = new ComponentMesh(this);
		itsNew = true;
	
		break;
	case CAMERA:
		
		ret = new ComponentCamera(this);
		itsNew = true;

		break;
	case type_comp::BB:
		BB = new ComponentBB(this);
		itsNew = true;
		ret = BB;

		break;
	
	}

	if (itsNew) {
		components.push_back(ret);
		ret->my_go = this;
	}

	return ret;

}

/*
void GameObject::CreateSphere(float r, int subdivisions) {
	par_shapes_mesh* sphere = par_shapes_create_subdivided_sphere(subdivisions);
	par_shapes_scale(sphere, r, r, r);
	par_shapes_translate(sphere, 0, 0, 0);

	App->renderer->CreateComponentMesh(this, sphere);
}
*/

int GameObject::GetChildIndexByUID(unsigned uid) {
	for (int i = 0; i < children.size(); ++i) if (children[i]->UID == uid) return i;
	return -1;
}

void GameObject::MoveToNewParent(GameObject* newParent) {

	if (newParent != parent && newParent != nullptr)
	{
		GameObject* lastParent = this->parent;

		bool isChild = false;
		GameObject* curr = newParent;
		while (curr->parent != nullptr)
		{
			if (curr->parent == this)
				return; //If we are trying to insert a parent object inside one of its childs, ignore it
			curr = curr->parent;
		}

		if (parent != nullptr)
		{
			parent->children.erase(parent->children.begin() + parent->GetChildIndexByUID(this->UID));
			parent->child_selected = false;
		}

		parent = newParent;
		if (newParent != nullptr) parentUID = newParent->UID;
		parent->children.push_back(this);
		parent->child_selected = true;

		transform->ParentChanged(lastParent);
		
	}
}


void GameObject::Save(JSON_Value* gameObjectsJSON) {

	JSON_Value* gameObject = gameObjectsJSON->createValue();

	gameObject->addUint("UID", UID);
	gameObject->addUint("ParentUID", (parent == App->scene->ROOT || parent == nullptr) ? 0 : parent->UID);
	gameObject->addString("Name", name);
	gameObject->addBool("Active", active);
	//gameObject->addBool("Static", isStatic);

	JSON_Value* ComponentsJSON = gameObjectsJSON->createValue();
	ComponentsJSON->convertToArray();

	for (int i = 0; i < components.size(); ++i) components[i]->Save(ComponentsJSON);

	gameObject->addValue("Components", ComponentsJSON);

	gameObjectsJSON->addValue("", gameObject);

	for (int i = 0; i < this->children.size(); ++i) this->children[i]->Save(gameObjectsJSON);
}

uint GameObject::Load(JSON_Value* gameObjectJSON) {

	this->UID = gameObjectJSON->getUint("UID");
	this->parentUID = gameObjectJSON->getUint("ParentUID");
	std::string strAux = gameObjectJSON->getString("Name");
	this->name = new char[strAux.length()];//&strAux[0u];
	strAux.copy(this->name, strAux.length());
	this->active = gameObjectJSON->getBool("Active");

	JSON_Value* Components = gameObjectJSON->getValue("Components"); //It is an array of values
	if (Components->getRapidJSONValue()->IsArray()) //Just make sure
	{
		for (int i = 0; i < Components->getRapidJSONValue()->Size(); i++)
		{
			JSON_Value* componentJSON = Components->getValueFromArray(i); //Get the component data
			Component* newComp = CreateComponent((type_comp)componentJSON->getInt("Type")); //Create the component type
			newComp->Load(componentJSON); //Load its info
		}
	}
	
	return this->UID;

}


