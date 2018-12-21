#include "Brofiler.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleRender.h"
#include <queue>
#include "ComponentTransform.h"
#include "ComponentBB.h"
#include "ComponentCamera.h"
#include <vector>
#include "Quadtree.h"
#include "JSONManager.h"

ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{

}

update_status ModuleScene::Update() {
	
	BROFILER_CATEGORY("Component Updates", Profiler::Color::Orchid);

	ROOT->Update();
	
	if (showQuad) quadTree->Draw();

	return UPDATE_CONTINUE;
}

void ModuleScene::NewGOSelected(GameObject* newGO) {

	if (GO_selected != nullptr) GO_selected->selected = false;

	GO_selected = newGO;
	newGO->SelectGO(true);
}

void ModuleScene::NewGameObject(char* name) {
	if (name == "") name = "Untittled";
	GameObject* newGo = new GameObject(name, true, GO_selected);
	GO_selected->children.push_back(newGo);	

	gameObjects[newGo->UID] = newGo;

}

GameObject* ModuleScene::CreateModel(char* name, GameObject* parent, char * path) {
	
	GameObject* GO = App->renderer->CreateModel(path);
	GO->name = name;
	GO->parent = parent;
	
	std::vector<Component*> comps = GO->GetComponents(MESH);
	GO->BB->SetAABB((std::vector<ComponentMesh*>*) &comps);
	parent->children.push_back(GO);
	gameObjects[GO->UID] = GO;

	return GO;
}

bool ModuleScene::Init() {
	ROOT = new GameObject("ROOT", true, nullptr);
	
	GO_selected = ROOT;

	float quadTreeSize = 20.0f;
	quadTree = new Quadtree(nullptr, float3(-quadTreeSize), float3 (quadTreeSize), 2, 6);

	GameObject* casa1 = CreateModel("Casa1", ROOT, "BakerHouse.fbx");
	quadTree->Insert(casa1);

	GameObject* casa2 = CreateModel("Casa2", ROOT, "BakerHouse.fbx");
	quadTree->Insert(casa2);

	GameObject* casa3 = CreateModel("Casa3", ROOT, "BakerHouse.fbx");
	quadTree->Insert(casa3);

	GameObject* camObject = new GameObject("ObjectCamera", true, ROOT);
	gameObjects[camObject->UID] = camObject;
	ComponentCamera* camComp = new ComponentCamera(camObject);
	camObject->AddComponent(camComp);

	
	
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

GameObject* ModuleScene::getGOByID(unsigned uid) {
	return gameObjects[uid];
}

void ModuleScene::SaveScene() {

	JSON_File* scene = App->JSON_manager->openWriteFile("sceneDefault.tte");

	//bool ret = saveScene(scene, ROOT);

	JSON_Value* gameObjects = scene->createValue();
	gameObjects->convertToArray();

	for (int i = 0; i < ROOT->children.size(); ++i) ROOT->children[i]->Save(gameObjects);

	scene->addValue("Game Objects", gameObjects);

	scene->Write();
	App->JSON_manager->closeFile(scene);

}
