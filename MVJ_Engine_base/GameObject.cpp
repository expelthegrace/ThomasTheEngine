#include "GameObject.h"
#include "ModuleRender.h"
#include "Application.h"
#include "Component.h"
#include "ComponentMesh.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

Component* GameObject::CreateComponent(type_comp type, int id, char * path) {
	Component* comp;

	switch (type) {
	case MESH:
		comp = App->renderer->CreateComponentMesh(this, id, path);		
		break;

	}

	

}
