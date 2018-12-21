#ifndef _APP_
#define _APP_

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Timers.h"

class JSONManager;
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
class ModuleScene;
class ModuleDebugDraw;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	unsigned generateUID();

public:
	JSONManager* JSONmanager = nullptr;

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
	ModuleScene* scene = nullptr;
	ModuleDebugDraw* debugdraw = nullptr;

	bool exit;

private:
	std::list<Module*> modules;

};

extern Application* App;

#endif