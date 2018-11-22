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
		if (renderTexture) glUniform1i(drawText, 1);
		else  glUniform1i(drawText, 0);
		

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

void ModuleRender::RenderBB(ComponentBB* BB) {
	

	glUseProgram(App->shaderProgram->programLines);

	float3* corners = new float3[8];


	int linesGrid = glGetUniformLocation(App->shaderProgram->programLines, "color0");
	float green[4] = { 0.2, 1, 0.2, 1 };
	glUniform4fv(linesGrid, 1, green);

	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, 24, corners, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLushort elements[] = {
	0, 1, 2, 
	4, 5, 6, 
	0, 3, 1, 4, 2, 5, 3, 6
	};
	GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programModel,
		"model"), 1, GL_TRUE, &BB->my_go->transform->model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programModel,
		"view"), 1, GL_TRUE, &App->camera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programModel,
		"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,  // attribute
		3,                  // number of elements per vertex, here (x,y,z,w)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &ibo_elements);
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

	return UPDATE_CONTINUE;
}

void ModuleRender::DrawGrid() {

	float3 colorWhite = { 1.,1.,1. };

	glUseProgram(App->shaderProgram->programLines);

	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programLines,
		"model"), 1, GL_TRUE, &identity[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programLines,
		"view"), 1, GL_TRUE, &App->camera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programLines,
		"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);

	int linesGrid = glGetUniformLocation(App->shaderProgram->programLines, "color0");
	float white[4] = { 1, 1, 1, 1 };
	glUniform4fv(linesGrid, 1, white);

	glLineWidth(1.0f);

	if (showGrid) {
		glBegin(GL_LINES);
		float d = 200.f;

		for (float i = -d; i <= d; i += 1.0f) {
			glVertex3f(i, 0.0f, -d);
			glVertex3f(i, 0.0f, d);
			glVertex3f(-d, 0.0f, i);
			glVertex3f(d, 0.0f, i);
		}
		glEnd();
	}

	glUseProgram(0);

}
// Called every draw update
update_status ModuleRender::Update()
{
	
	for (int i = 0; i < meshComponents.size(); ++i) if (meshComponents[i]->active && meshComponents[i]->avaliable) RenderMesh(meshComponents[i]);

	DrawGrid();
	
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
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


