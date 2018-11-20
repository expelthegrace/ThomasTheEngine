#include "ModuleScene.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleRender.h"



ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{
}

GameObject* ModuleScene::CreateModel(char* name, GameObject* parent, char * path) {
	
	GameObject* GO = App->renderer->CreateModel(path);
	GO->name = name;
	GO->parent = parent;

	return nullptr;
}

bool ModuleScene::Init() {
	ROOT = new GameObject("ROOT", true, nullptr);
	gameObjects.push_back(ROOT);

	CreateModel("casa", ROOT, "BakerHouse.fbx");

	return true;
}