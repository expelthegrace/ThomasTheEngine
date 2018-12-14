#ifndef _MODULERENDER_
#define _MODULERENDER_

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include <vector>


struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
struct FBOset;

class ComponentMesh;
class GameObject;
class ComponentBB;
class ComponentCamera;

struct Mesh;
class ComponentMaterial;


class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	void GenerateFBOTexture(unsigned w, unsigned h, FBOset* fboset);


	void RemoveCamera(ComponentCamera* cam);

	void DrawGrid();

	ComponentMesh* CreateComponentMesh(GameObject* my_go);
	ComponentMesh* CreateComponentMesh(GameObject* my_go,int idMesh, char* path);
	GameObject* CreateModel(char* path);

	ComponentMaterial * CreateComponentMaterial(GameObject* my_go, int idMesh, char* path);

private:
	update_status RenderMesh(ComponentMesh * meshComp);
	void DrawCameras();

public:
	math::float4x4 identity;

	bool renderTexture, showGrid;
	void* context;

	std::vector<ComponentCamera*> cameras;

private:
	std::vector<ComponentMesh*> meshComponents;
};

#endif