#include "GameObject.h"
#include "ModuleRender.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentBB.h"


GameObject::GameObject(char * name, bool active, GameObject * parent)
{
	this->name = name;
	this->active = active;
	this->parent = parent;
	this->lastFrameActive = this->active;

	ComponentBB* BB = new ComponentBB(this);
	this->BB = BB;
	components.push_back(BB);

	transform = new ComponentTransform();
	transform->my_go = this;
	components.push_back(transform);
}


GameObject::~GameObject()
{
	delete name;
	for (int i = 0; i < components.size(); ++i) delete components[i];
	for (int i = 0; i < children.size(); ++i) delete children[i];

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

Component* GameObject::CreateComponent(type_comp type, int id, char * path) {
	Component* comp;



	return comp;
}


