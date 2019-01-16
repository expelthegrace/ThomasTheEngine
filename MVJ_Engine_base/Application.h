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
class ModuleGuizmo;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	unsigned generateUID();
	void SaveDefaultConfig(const char* path);

public:
	JSONManager* JSON_manager = nullptr;

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
	ModuleGuizmo* guizmo = nullptr;

	bool exit;

	float GameScale, GameScaleIni = 1.f;

private:
	std::list<Module*> modules;

};

extern Application* App;

#endif