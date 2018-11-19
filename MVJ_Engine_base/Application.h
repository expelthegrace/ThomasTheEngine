#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Timers.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleMenu;
class ModuleCamera;
class ModuleModelLoader;
class Timer;
class ModuleTimer;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
    ModuleRenderExercise* exercise = nullptr;
	ModuleProgram* shaderProgram = nullptr;
	ModuleMenu* menu = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleModelLoader* modelLoader = nullptr;
	ModuleTimer* timer = nullptr;

	bool exit;

private:
	std::list<Module*> modules;

};

extern Application* App;
