#include "ModuleScene.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleRender.h"
#include <queue>
#include "ComponentTransform.h"
#include "ComponentBB.h"
#include <vector>


ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{
}

update_status ModuleScene::Update() {
	//Update all transforms
	ROOT->Update();
	//for (int i = 0; i < ROOT->children.size(); ++i)  ROOT->children[i]->transform->Update();

	return UPDATE_CONTINUE;
}

GameObject* ModuleScene::CreateModel(char* name, GameObject* parent, char * path) {
	
	GameObject* GO = App->renderer->CreateModel(path);
	GO->name = name;
	GO->parent = parent;
	
	std::vector<Component*> comps = GO->GetComponents(MESH);
	std::vector<Component*>* components = & comps;
	GO->BB->SetAABB((std::vector<ComponentMesh*>*) components);

	parent->children.push_back(GO);

	return nullptr;
}

bool ModuleScene::Init() {
	ROOT = new GameObject("ROOT", true, nullptr);
	//gameObjects.push_back(ROOT);
	GameObject* casa = CreateModel("casa", ROOT, "BakerHouse.fbx");
	

	return true;
}

/**Find game object by name **/
GameObject* ModuleScene::FindByName(char * name) {

	std::queue<GameObject*> GOqueue;
	GOqueue.push(ROOT);

	while (!GOqueue.empty()) {
		GameObject* GOactual = GOqueue.front();
		GOqueue.pop();
		if (GOactual->name == name) return GOactual;
		for (int i = 0; i < GOactual->children.size(); ++i) GOqueue.push(GOactual->children[i]);
	}
	return nullptr;
}

void ModuleScene::MoveTo(GameObject* source, GameObject* newParent) {

}
