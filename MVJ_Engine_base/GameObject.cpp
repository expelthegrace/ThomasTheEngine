#include "GameObject.h"
#include "ModuleRender.h"
#include "Application.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

Component* GameObject::CreateComponent(type_comp type) {

	Component * comp;

	switch (type) {
	case (MESH):

		//comp = App->renderer->CreateComponentMesh()


		break;

	}

}
