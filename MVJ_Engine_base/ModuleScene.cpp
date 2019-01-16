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
#include "ComponentMaterial.h"
#include <vector>
#include "Quadtree.h"
#include "JSONManager.h"
#include <string>
#include "debugdraw.h"
#include "ModuleDebugDraw.h"

using namespace std;

ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{

}

update_status ModuleScene::Update() {
	
	BROFILER_CATEGORY("Component Updates", Profiler::Color::Orchid);

//	if (App->input->mouse_buttons[SDL_BUTTON_LEFT - 1] == KEY_DOWN) mouseClick(App->input->mouse_position.x, App->input->mouse_position.y);

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
	if (parent == nullptr) GO->parent = ROOT;
	GO->parent->children.push_back(GO);
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

	float quadTreeSize = 20.0f * App->GameScale;
	quadTree = new Quadtree(nullptr, float3(-quadTreeSize), float3 (quadTreeSize), 5, 8);

	LoadScene();

//	GameObject* casa1 = CreateModel("Casa1", ROOT, "BakerHouse.fbx");
//	quadTree->Insert(casa1);
//
//	//GameObject* casa2 = CreateModel("Casa2", ROOT, "BakerHouse.fbx");
//	//quadTree->Insert(casa2);
//
//	//GameObject* casa3 = CreateModel("Casa3", ROOT, "BakerHouse.fbx");
//	//quadTree->Insert(casa3);
//
//	/*GameObject* bunny1 = CreateModel("Bunny1", ROOT, "Assets/Zombunny.fbx");
//	quadTree->Insert(bunny1);
//*/
//	
//
//	GameObject* camObject = new GameObject("ObjectCamera", true, ROOT);
//	gameObjects[camObject->UID] = camObject;
//	ComponentCamera* camComp = new ComponentCamera(camObject);
//	camObject->AddComponent(camComp);
//
//	GameObject* lightObject = new GameObject("ObjectLight", true, ROOT);
//	gameObjects[lightObject->UID] = lightObject;
//	lightObject->CreateComponent(LIGHT);

	
	
	return true;
}

void ModuleScene::DragInputManager(char * path) {
	string pathString = path;

	for (int i = 0; i < pathString.length(); i++)
	{
		if (pathString[i] == '\\')
			pathString[i] = '/';
	}

	uint pos_slash = pathString.find_last_of('/');
	uint pos_dot = pathString.find_last_of('.');

	string extension = pathString.substr(pos_dot + 1);
	string name = pathString.substr(pos_slash + 1, pos_dot - pos_slash - 1);

	if (extension == "fbx" || extension == "FBX" || extension == "obj" || extension == "OBJ")
	{
		char * newName = new char[name.size() + 1];
		strcpy(newName, name.c_str());
		CreateModel(newName, App->scene->ROOT, path);
	}

	else if (extension == "png" || extension == "dds" || extension == "tga")
	{
		if (GO_selected->material != nullptr) GO_selected->material->LoadTexture(path);
	}
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
	if (ret == nullptr) {
		gameObjects.erase(uid);
		return ROOT;
	}
	return ret;
}

void ModuleScene::ClearScene() {
	quadTree->Clear();
	for (int i = 0; i < ROOT->children.size(); ++i) RELEASE( ROOT->children[i]);
	
	ROOT->children.clear();
	gameObjects.clear();
	GO_selected = ROOT;
	mainCamera = nullptr;
	mainLight = nullptr;
}

void ModuleScene::mouseClick(int mouseXi, int mouseYi) {
	float mouseX = (float)mouseXi;
	float mouseY = (float)mouseYi;
	math::float2 viewportTopLeft(App->menu->columnWidth, 20.0f);
	math::float2 viewportSize(App->camera->screenWidth - 2 * App->menu->columnWidth, App->camera->screenHeight - App->menu->consoleHeight - 20);
	math::float2 windowSize(App->camera->screenWidth, App->camera->screenHeight);
	float endpointX = App->menu->columnWidth + App->camera->screenWidth - 2 * App->menu->columnWidth;
	float endpointY = 20.0f + App->camera->screenHeight - App->menu->consoleHeight - 20;
	if (mouseX > App->menu->columnWidth && mouseX < (endpointX) &&
		mouseY > 20.0f && mouseY < endpointY) {
		float sy, sx, ty, tx;
		/*sy = (1 + 1) / (-viewportSize.y);
		sx = (1 + 1) / (viewportSize.x);
		ty = (-(viewportSize.y + viewportTopLeft.y) - viewportTopLeft.y) / (-viewportSize.y);
		tx = (-(viewportSize.x + viewportTopLeft.x) - viewportTopLeft.x) / (viewportSize.x);*/

		sy = (1 + 1) / (-windowSize.y);
		sx = (1 + 1) / (windowSize.x);
		ty = (-(windowSize.y + viewportTopLeft.y) - viewportTopLeft.y) / (-windowSize.y);
		tx = (-(windowSize.x + viewportTopLeft.x) - viewportTopLeft.x) / (windowSize.x);

		float normX = sx * (mouseX)+tx - 0.01;
		float normY = sy * (mouseY)+ty + 0.06;
		ray = App->camera->frustum.UnProjectLineSegment(normX, normY);

		//future implementation: make quadtree work fully and use it to make this algorithm more efficient
		//now we check collisions

		map<unsigned int, GameObject*>::iterator it;
		std::vector<GameObject*> collisions;
		for (it = gameObjects.begin(); it != gameObjects.end(); ++it) {
			if (it->second->BB != nullptr /*it->second->BB->Aabb->IsFinite() && ray.Intersects(*(it->second->BB->Aabb))*/) {
				collisions.push_back(it->second);
			}
		}

		GameObject* picked = nullptr;
		if (collisions.size() > 0) picked = collisions[0];
		for (int i = 0; i < collisions.size(); ++i) {
			picked = closestToCam(picked, collisions[i]);
		}
		//now that we have the picked object, make it selected
		if (picked != nullptr) {
			NewGOSelected(picked);
		}

		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				float newStartX = App->scene->ray.a.x + 0.001*i;
				float newStartY = App->scene->ray.a.y + 0.001*j;
				float newEndX = App->scene->ray.b.x + 0.001*i;
				float newEndY = App->scene->ray.b.y + 0.001*j;
				math:float3 newStart(newStartX, newStartY, App->scene->ray.a.z);
				math::float3 newEnd(newEndX, newEndY, App->scene->ray.b.z);

				dd::line(newStart, newEnd, math::float3(0.8, 0.3, 0.3));
			}
		}
	}
}

GameObject* ModuleScene::closestToCam(GameObject* go1, GameObject* go2) {
	if (go1->BB->Aabb->Distance(App->camera->frustum.pos) < go2->BB->Aabb->Distance(App->camera->frustum.pos)) return go1;
	else return go2;
}

void ModuleScene::SaveScene() {

	JSON_File* scene = App->JSON_manager->openWriteFile(scenePath);


	JSON_Value* gameObjectsJSON = scene->createValue();
	gameObjectsJSON->convertToArray();
	
	for (int i = 0; i < ROOT->children.size(); ++i) ROOT->children[i]->Save(gameObjectsJSON);

	scene->addValue("GameObjects", gameObjectsJSON);
	
	//Scene Properties
	JSON_Value* sceneProperties = scene->createValue();
	sceneProperties->addFloat("GameScale", App->GameScale);
	scene->addValue("Scene_properties", sceneProperties);

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
		App->GameScale = sceneJSON->getValue("Scene_properties")->getFloat("GameScale");
		App->GameScaleIni = App->GameScale;

		float quadTreeSize = 20.0f * App->GameScale;
		quadTree->Resize(float3(-quadTreeSize), float3(quadTreeSize));

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

		App->camera->UpdateFrustum();

		char* b = new char[50];
		sprintf(b, "-- %s loaded --\n", App->scene->scenePath);
		App->menu->console.AddLog(b);

	}

}
