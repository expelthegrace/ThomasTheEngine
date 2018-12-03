#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "ModuleRenderExercise.h"
#include "ModuleModelLoader.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include "ModuleMenu.h"
#include "ComponentBB.h"
#include "debugdraw.h"
#include "ModuleDebugDraw.h"
#include "ModuleScene.h"
#include "Brofiler.h"

ModuleRender::ModuleRender()
{
}


ModuleRender::~ModuleRender()
{
}


ComponentMesh* ModuleRender::CreateComponentMesh( GameObject* my_go) {

	ComponentMesh* meshComp = new ComponentMesh(my_go);
	meshComponents.push_back(meshComp);
	return meshComp;
}

ComponentMesh* ModuleRender::CreateComponentMesh(GameObject* my_go, int idMesh, char* path) {

	ComponentMesh* meshComp = new ComponentMesh(my_go);
	App->modelLoader->GenerateMesh(idMesh, meshComp, path);
	
	if (meshComp->mesh.numVertices > 0) meshComp->avaliable = true;
	else {
		char* b = new char[50];
		sprintf(b, "A mesh of <%s> has 0 vertices. It will not be rendered\n",my_go->name);
		App->menu->console.AddLog(b);
	}

	meshComponents.push_back(meshComp); // saved in the render module
	return meshComp;
}

ComponentMaterial* ModuleRender::CreateComponentMaterial(GameObject* my_go, int idMaterial, char* path) {
	unsigned mat = App->modelLoader->GenerateMaterial(idMaterial, path);
	ComponentMaterial* materialComp = new ComponentMaterial(mat);

	return materialComp;
}

GameObject* ModuleRender::CreateModel(char * path) {
	GameObject* newGO = new GameObject("", true, nullptr);

	const aiScene* sceneAct = aiImportFile(path, aiProcess_Triangulate);

	for (int i = 0; i < sceneAct->mNumMaterials; ++i) newGO->components.push_back(CreateComponentMaterial(newGO, i, path));
	for (int i = 0; i < sceneAct->mNumMeshes; ++i) newGO->components.push_back(CreateComponentMesh(newGO, i, path));
	
	return newGO;
}

update_status ModuleRender::RenderMesh(ComponentMesh* meshComp) {
	
	Mesh meshActual = meshComp->mesh;

	if (meshActual.numVertices > 0) {
		glUseProgram(App->shaderProgram->programModel);


		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programModel,
			"model"), 1, GL_TRUE, &meshComp->my_go->transform->model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programModel,
			"view"), 1, GL_TRUE, &App->camera->view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programModel,
			"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);


		GLint drawText = glGetUniformLocation(App->shaderProgram->programModel, "drawTexture");
		GLint color0 = glGetUniformLocation(App->shaderProgram->programModel, "color0");

		if (meshComp->renderTexture) glUniform1i(drawText, 1);
		else {
			glUniform1i(drawText, 0);
			float color[4] = { 0.6, 1, 0.6, 1 };
			glUniform4fv(color0, 1, color);
		}
		

		unsigned vboActual = meshComp->mesh.vbo; //           <------------------ correct
		unsigned numVerticesActual = meshActual.numVertices;
		unsigned numIndexesActual = meshActual.numIndexesMesh; 

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meshComp->mesh.materialIndex );// App->modelLoader->materials[App->modelLoader->textures[i]]); // <- mesh.materialIndex
		glUniform1i(glGetUniformLocation(App->shaderProgram->programModel, "texture0"), 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboActual);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * numVerticesActual));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshComp->mesh.ibo);
		glDrawElements(GL_TRIANGLES, numIndexesActual, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);


		glUseProgram(0);

	} 
	return UPDATE_CONTINUE;
}


// Called before render is available
bool ModuleRender::Init()
{
	identity = float4x4::identity;
	renderTexture = true;
	showGrid = true;
	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->window->window);

	glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);

    int width, height;
    SDL_GetWindowSize(App->window->window, &width, &height);
    glViewport(0, 0, width, height);


	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	App->scene->ROOT->transform->UpdateTransform(false);

	return UPDATE_CONTINUE;
}

void ModuleRender::DrawGrid() {
	
	glUseProgram(App->shaderProgram->programLines);

	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programLines,
		"model"), 1, GL_TRUE, &identity[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programLines,
		"view"), 1, GL_TRUE, &App->camera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programLines,
		"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);

	// Grid
	const float3 white2 = { 1.f,1.f,1.f};
	dd::xzSquareGrid(-100, 100, 0, 1, white2);
	// Center Axis
	const float4x4 identity_mat = float4x4::identity;
	dd::axisTriad(identity_mat, 0.2f, 4.0f);

	glUseProgram(0);


}
// Called every draw update
update_status ModuleRender::Update()
{
	//draw editor camera
	ModuleCamera* cam = App->camera;
	GenerateFBOTexture(cam->editorWidth, cam->editorHeight, &(cam->fboSet));

	glBindFramebuffer(GL_FRAMEBUFFER, cam->fboSet.fbo);
	glViewport(0, 0, cam->fboSet.fb_width, cam->fboSet.fb_height); 
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	BROFILER_CATEGORY("Render Meshes", Profiler::Color::Orchid);
	for (int i = 0; i < meshComponents.size(); ++i) if (
		meshComponents[i]->active &&
		meshComponents[i]->avaliable &&
		meshComponents[i]->my_go->active)
		RenderMesh(meshComponents[i]);

	DrawGrid();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	App->debugdraw->Draw(nullptr, &(App->camera->fboSet));

	App->menu->DrawEditorCamera();
	
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}


void ModuleRender::GenerateFBOTexture(unsigned w, unsigned h, FBOset* fboset)
{

	if (w != fboset->fb_width || h != fboset->fb_height)
	{
		if (fboset->fb_tex != 0)
		{
			glDeleteTextures(1, &(fboset->fb_tex));
		}

		if (w != 0 && h != 0)
		{
			if (fboset->fbo == 0)
			{
				glGenFramebuffers(1, &(fboset->fbo));
			}

			glBindFramebuffer(GL_FRAMEBUFFER, fboset->fbo);
			glGenTextures(1, &(fboset->fb_tex));
			glBindTexture(GL_TEXTURE_2D, fboset->fb_tex);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glGenRenderbuffers(1, &(fboset->fb_depth));
			glBindRenderbuffer(GL_RENDERBUFFER, fboset->fb_depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboset->fb_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboset->fb_tex, 0);

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		fboset->fb_width = w;
		fboset->fb_height = h;
	}

}

// Called before quitting
bool ModuleRender::CleanUp()
{
	for (int i = 0; i < meshComponents.size(); ++i)	delete meshComponents[i];
	meshComponents.clear();


	LOG("Destroying renderer");
	SDL_GL_DeleteContext(context);

	//Destroy window

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
    glViewport(0, 0, width, height); 
	App->camera->ResizeFOV(width,height);
}


