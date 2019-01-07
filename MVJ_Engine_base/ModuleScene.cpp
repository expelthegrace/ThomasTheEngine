#include "Brofiler.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleMenu.h"
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

	if (App->input->keyboard[SDL_SCANCODE_DELETE]) DeleteSelected();


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
	parent->children.push_back(GO);
	gameObjects[GO->UID] = GO;

	return GO;
}

void ModuleScene::DeleteSelected() {
	if (GO_selected != nullptr && GO_selected != ROOT) {

		quadTree->RemoveAndMerge(GO_selected);
		gameObjects.erase(GO_selected->UID);

		GO_selected->RemoveFromParent();

		delete GO_selected;
		NewGOSelected(ROOT);
	}
}

void ModuleScene::DuplicateSelected() {
	if (GO_selected != nullptr && GO_selected != ROOT) {

		 
	}
}

bool ModuleScene::Init() {
	ROOT = new GameObject("ROOT", true, nullptr);
	
	GO_selected = ROOT;

	float quadTreeSize = 20.0f;
	quadTree = new Quadtree(nullptr, float3(-quadTreeSize), float3 (quadTreeSize), 2, 8);

	GameObject* casa1 = CreateModel("Casa1", ROOT, "BakerHouse.fbx");
	quadTree->Insert(casa1);

	//GameObject* casa2 = CreateModel("Casa2", ROOT, "BakerHouse.fbx");
	//quadTree->Insert(casa2);

	//GameObject* casa3 = CreateModel("Casa3", ROOT, "BakerHouse.fbx");
	//quadTree->Insert(casa3);

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
	GameObject* ret = gameObjects[uid];
	if (ret == nullptr) return ROOT;
	return ret;
}

void ModuleScene::ClearScene() {
	quadTree->Clear();
	for (int i = 0; i < ROOT->children.size(); ++i) delete ROOT->children[i];
	
	ROOT->children.clear();
	gameObjects.clear();
	GO_selected = ROOT;
	mainCamera = nullptr;
}

void ModuleScene::SaveScene() {

	JSON_File* scene = App->JSON_manager->openWriteFile(scenePath);


	JSON_Value* gameObjectsJSON = scene->createValue();
	gameObjectsJSON->convertToArray();
	
	for (int i = 0; i < ROOT->children.size(); ++i) ROOT->children[i]->Save(gameObjectsJSON);

	scene->addValue("GameObjects", gameObjectsJSON);

	scene->Write();
	App->JSON_manager->closeFile(scene);

}

void ModuleScene::LoadScene() {
	
	ClearScene();

	JSON_File* sceneJSON = App->JSON_manager->openReadFile(scenePath);

	if (sceneJSON == nullptr) {
		char* b = new char[50];
		sprintf(b, "-- ERROR: %s not found, scene not loaded -- \n", scenePath);
		App->menu->console.AddLog(b);
	}
	else {

		JSON_Value* gameObjectsJSON = sceneJSON->getValue("GameObjects");

		if (gameObjectsJSON->getRapidJSONValue()->IsArray()) {

			for (int i = 0; i < gameObjectsJSON->getRapidJSONValue()->Size(); i++)
			{
				GameObject* GO = new GameObject();
				unsigned UIDTemp = GO->Load(gameObjectsJSON->getValueFromArray(i));
				gameObjects[UIDTemp] = GO;
			}

			for (std::map<unsigned, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) {
				GameObject * parentAux = getGOByID(it->second->parentUID);
				it->second->parent = parentAux;
				parentAux->AddChild(it->second);

			}
					
		
		}

		char* b = new char[50];
		sprintf(b, "-- %s loaded --\n", App->scene->scenePath);
		App->menu->console.AddLog(b);

	}

}
