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

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}


ComponentMesh* ModuleRender::CreateComponentMesh( GameObject* my_go) {

	ComponentMesh* meshComp = new ComponentMesh(my_go);
	meshComponents.push_back(meshComp);
	return meshComp;
}

ComponentMesh* ModuleRender::CreateComponentMesh(GameObject* my_go,int idMesh, char* path) {
	
	Mesh meshAux = App->modelLoader->GenerateMesh(idMesh, path);
	ComponentMesh* meshComp = new ComponentMesh(my_go,meshAux);

	if (meshAux.numVertices > 0) (*meshComp).mesh = meshAux;
	else meshComp->avaliable = false;

	meshComponents.push_back(meshComp);
	return meshComp;
}

// Called before render is available
bool ModuleRender::Init()
{
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


	model = math::float4x4::identity;

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	if (App->modelLoader->scene != NULL) {
		glUseProgram(App->shaderProgram->programModel);

		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programModel,
			"model"), 1, GL_TRUE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programModel,
			"view"), 1, GL_TRUE, &App->camera->view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programModel,
			"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);


		GLint drawText = glGetUniformLocation(App->shaderProgram->programModel, "drawTexture");
		if (renderTexture) glUniform1i(drawText, 1);
		else  glUniform1i(drawText, 0);

		for (int i = 0; i < App->modelLoader->scene->mNumMeshes; ++i) {


			unsigned vboActual = App->modelLoader->vbos[i];
			unsigned numVerticesActual = App->modelLoader->numVerticesMesh[i];
			unsigned numIndexesActual = App->modelLoader->numIndexesMesh[i];

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, App->modelLoader->materials[App->modelLoader->textures[i]]);
			glUniform1i(glGetUniformLocation(App->shaderProgram->programModel, "texture0"), 0);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, vboActual);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * App->modelLoader->numVerticesMesh[i]));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->modelLoader->ibos[i]);
			glDrawElements(GL_TRIANGLES, numIndexesActual, GL_UNSIGNED_INT, nullptr);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

		}
		glUseProgram(0);
	}
	float3 colorWhite = { 1.,1.,1. };

	glUseProgram(App->shaderProgram->programLines);

	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programLines,
		"model"), 1, GL_TRUE, &model[0][0]);
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


