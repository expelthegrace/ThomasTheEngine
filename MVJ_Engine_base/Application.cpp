#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleMenu.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "ModuleTimer.h"
#include "ModuleScene.h"
#include "ModuleDebugDraw.h"
#include "Brofiler.h"
#include "pcg_variants.h"
#include "entropy.h"
#include <time.h>
#include "JSONManager.h"
#include <string>
#include <stdio.h>


pcg32_random_t rng;

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(menu = new ModuleMenu());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(modelLoader = new ModuleModelLoader());
	modules.push_back(shaderProgram = new ModuleProgram());
	modules.push_back(timer = new ModuleTimer());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(debugdraw = new ModuleDebugDraw());
		

}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;
	exit = false;

	//std::string path = "./";
	//path += "config.json";

	//JSON_File* document = JSON_manager->openReadFile(path.c_str());

	//if (document == nullptr)
	//{
	//	SaveDefaultConfig(path.c_str());
	//	document = JSON_manager->openReadFile(path.c_str());
	//}
	//else 
	//{

	//	//// Call Init() in all modules
	//	//for (list<Module*>::iterator item = modules.begin(); item != modules.end() && ret == true; item++)
	//	//{
	//	//	ret = (*item)->Init(document);
	//	//}
	//}


	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();
	

	return ret;
}

void Application::SaveDefaultConfig(const char* path) {

	JSON_File* document = JSON_manager->openWriteFile(path);

	JSON_Value* app = document->createValue();
	document->addValue("App", app);

	JSON_Value* scena1 = document->createValue();
	document->addValue("scene1", scena1);

	document->Write();
	JSON_manager->closeFile(document);
}

unsigned Application::generateUID() {
	return pcg32_random_r(&rng);
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	
	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	if (exit) ret = UPDATE_STOP;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
