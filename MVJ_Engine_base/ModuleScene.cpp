#include "Brofiler.h"
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
	
	BROFILER_CATEGORY("Component Updates", Profiler::Color::Orchid);

	ROOT->Update();

	return UPDATE_CONTINUE;
}

void ModuleScene::NewGOSelected(GameObject* newGO) {

	if (GO_selected != nullptr) GO_selected->selected = false;

	GO_selected = newGO;
	newGO->SelectGO(true);
}

GameObject* ModuleScene::CreateModel(char* name, GameObject* parent, char * path) {
	
	GameObject* GO = App->renderer->CreateModel(path);
	GO->name = name;
	GO->parent = parent;
	
	std::vector<Component*> comps = GO->GetComponents(MESH);
	GO->BB->SetAABB((std::vector<ComponentMesh*>*) &comps);
	parent->children.push_back(GO);

	return GO;
}

bool ModuleScene::Init() {
	ROOT = new GameObject("ROOT", true, nullptr);
	
	GO_selected = ROOT;

	GameObject* casa1 = CreateModel("Casa1", ROOT, "BakerHouse.fbx");
	GameObject* casa2 = CreateModel("Casa2", casa1, "BakerHouse.fbx");
	
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
