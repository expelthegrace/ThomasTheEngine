#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleMenu.h"
#include "ModuleRender.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include <stdio.h>
#include <SDL.h>
#include "ModuleRenderExercise.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleModelLoader.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include <vector>



ModuleMenu::ModuleMenu()
{
}


ModuleMenu::~ModuleMenu()
{
}

bool ModuleMenu::Init() {
	b = new char[50];

	// Setup window
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	//setting up the vars for the logs
	logMSIterator = 0;
	logFPSIterator = 0;
	logMemoryIterator = 0;
	fps_log = new float[50];
	ms_log = new float[50];
	memory_log = new float[50];
	lastFrameTime = SDL_GetTicks();
	lastSecondTime = SDL_GetTicks();
	
	showWindows = true;
	ImGui::StyleColorsDark();

	ImVec4 olive = { 0.6f,0.6f,0.0f,1.0f };
	ImVec4 oliveLighter = { 0.7f,0.7f,0.1f,1.0f };
	ImVec4 oliveLTrans = { 0.5f,0.5f,0.f,.7f };

	columnScreenRatio = 6.f / 8.5f;

	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = { 0.1f,0.1f,0.1f,0.7f };
	//ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] = { 0.05f, 0.05f,0.0f,1.0f };
	//ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = { 0.05f, 0.05f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = { 0.2f, 0.2f,0.1f,1.0f };

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Header] = olive;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_HeaderHovered] = oliveLighter;

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Button] = olive;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_ButtonHovered] = oliveLighter;

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBg] = oliveLTrans;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBgActive] = oliveLTrans;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBgHovered] = oliveLTrans;
	/*
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Tab] = { 0.3f,0.7f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_TabHovered] = { 0.8f,0.35f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_TabUnfocused] = { 0.5f,0.2f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_TabUnfocusedActive] = { 0.65f,0.25f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_TabActive] = { 0.9f,0.45f,0.0f,1.0f };

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Header] = { 0.8f,0.37f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_HeaderActive] = { 1.0f,0.6f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_HeaderHovered] = { 0.95f,0.5f,0.0f,1.0f };

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_Button] = { 0.9f,0.45f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_ButtonActive] = { 0.8f,0.37f,0.0f,1.0f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_ButtonHovered] = { 0.95f,0.5f,0.0f,1.0f };

	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBg] = { 1.0f,0.6f,0.0f,0.2f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBgActive] = { 0.8f,0.37f,0.0f,0.5f };
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBgHovered] = { 0.95f,0.5f,0.0f,0.5f };
	*/


	return true;
}

void ModuleMenu::updateFramerates() {
	double timeElapsed = SDL_GetTicks() - lastSecondTime;
	//fps calculation
	lastSecondTime = SDL_GetTicks();
	fps_log[logFPSIterator] = 1000/timeElapsed;
	++logFPSIterator;
	if (logFPSIterator > 49) logFPSIterator = 0;
	//ms calculation
	ms_log[logMSIterator] = timeElapsed;
	lastFrameTime = SDL_GetTicks();

	memory_log[logMemoryIterator] = (float)pmc.WorkingSetSize / 1000000;
	++logMemoryIterator;
	if (logMemoryIterator > 49) logMemoryIterator = 0;
	//iterator increase
	++logMSIterator;
	if (logMSIterator > 49) logMSIterator = 0;
}

void ModuleMenu::FillTree(GameObject* gameobject)
{
	unsigned int flags;

	flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (gameobject->children.size() <= 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 15);
	}


	if (!gameobject->active)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, { 0.7f,0.7f,0.7f,1.f });
	}

	if (gameobject->selected)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	if (gameobject->child_selected)
	{
		ImGui::SetNextTreeNodeOpen(true);
	}

	bool opened = ImGui::TreeNodeEx(gameobject->name, flags);
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("GAME_OBJECT", &gameobject->UID, sizeof(unsigned));
		ImGui::Text(gameobject->name);
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget())
	{
		bool isParent = false;
		if (const ImGuiPayload* prevPayload = ImGui::GetDragDropPayload())
		{
			GameObject* draggedGameobject = App->scene->getGOByID(*(unsigned*)prevPayload->Data);
			if (draggedGameobject != nullptr && draggedGameobject->parent == gameobject)
				isParent = true;
		}

		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAME_OBJECT", ImGuiDragDropFlags_SourceAllowNullID))
		{
			GameObject* draggedGameobject = App->scene->getGOByID(*(unsigned*)payload->Data);
			if (draggedGameobject != nullptr)
				draggedGameobject->MoveToNewParent((isParent) ? gameobject->parent : gameobject);
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::IsItemClicked(0))
	{
		App->scene->NewGOSelected(gameobject);
	}
	
	if (opened)
	{
		
		for (int i = 0; i < gameobject->children.size(); ++i)
		{
			FillTree(gameobject->children[i]);
		}
		
		ImGui::TreePop();
	}

	if (!gameobject->active)
	{
		ImGui::PopStyleColor();
	}
}

update_status ModuleMenu::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

void ModuleMenu::DrawEditorCamera() {

	bool obert = true;

	App->camera->editorWidth = (App->camera->screenWidth - 2 * columnWidth) / 2;
	App->camera->editorHeight = (App->camera->screenHeight - mainMenuSize.y) - consoleHeight;

	if (!cameraInitUpdated || fovChanged) {
		App->camera->UpdateFrustum();
		cameraInitUpdated = true;
	}

	ImGui::SetNextWindowPos(ImVec2(0, mainMenuSize.y));
	ImGui::SetNextWindowSize(ImVec2(App->camera->editorWidth, App->camera->editorHeight));

	if (ImGui::Begin("Editor")) {

		if (ImGui::BeginChild("Editor Canvas", ImVec2(0, 0), true, ImGuiWindowFlags_NoMove))
		{
			ImGui::GetWindowDrawList()->AddImage(
				(void*)App->camera->fboSet.fb_tex,
				ImVec2(ImGui::GetCursorScreenPos()),
				ImVec2(ImGui::GetCursorScreenPos().x + App->camera->fboSet.fb_width,
					ImGui::GetCursorScreenPos().y + App->camera->fboSet.fb_height),
				ImVec2(0, 1), ImVec2(1, 0));			
		}
		ImGui::EndChild();
	}
	ImGui::End();

}

void ModuleMenu::DrawMainCamera() {

	bool obert = true;


	if (App->scene->mainCamera != nullptr) {

		if (fovChanged)	App->scene->mainCamera->UpdateFrustum();

		ImGui::SetNextWindowPos(ImVec2(App->camera->editorWidth, mainMenuSize.y));
		ImGui::SetNextWindowSize(ImVec2(App->camera->editorWidth, App->camera->editorHeight));

		if (ImGui::Begin("Game")) {

			if (ImGui::BeginChild("Main Camera", ImVec2(0, 0), true, ImGuiWindowFlags_NoMove))
			{
				ImGui::GetWindowDrawList()->AddImage(
					(void*)App->scene->mainCamera->fboSet.fb_tex,
					ImVec2(ImGui::GetCursorScreenPos()),
					ImVec2(ImGui::GetCursorScreenPos().x + App->scene->mainCamera->fboSet.fb_width,
						ImGui::GetCursorScreenPos().y + App->scene->mainCamera->fboSet.fb_height),
					ImVec2(0, 1), ImVec2(1, 0));
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
}

void ModuleMenu::DrawCameras() {

	DrawEditorCamera();
	DrawMainCamera();

}

update_status ModuleMenu::MainBarMenu() {
	consoleHeight = App->camera->screenHeight * 1.f / 4;
	
	// Menu superior
	if (ImGui::BeginMainMenuBar())
	{
		mainMenuSize = ImGui::GetWindowSize();
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::Checkbox("Show windows", &showWindows);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About")) {
			bool openAbout = true;
			ImGui::Text("Engine name: Thomas The Engine (TTE) \nCreated by : Marc Palomar Soler \nProject for the UPC University Master");
			if (ImGui::Button("Go to GitHub")) {
				ShellExecute(NULL, "open", "https://github.com/expelthegrace/ThomasTheEngine", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Quit")) {
			return UPDATE_STOP;
		}

		ImGui::EndMainMenuBar();

	}
	return UPDATE_CONTINUE;
}

update_status ModuleMenu::Configuration() {
	bool obert = true;

	ImGui::SetNextWindowPos(ImVec2(App->camera->screenWidth - 2 * columnWidth, (App->camera->screenHeight - mainMenuSize.y) / 2 + mainMenuSize.y));
	ImGui::SetNextWindowSize(ImVec2(columnWidth, (App->camera->screenHeight - mainMenuSize.y) / 2));
	ImGui::Begin("Configuration", &obert);

	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse position: %i , %i", App->input->mouse_position.x, App->input->mouse_position.y);
		ImGui::Text("Mousewheel force: %i", App->input->mouseWheel);

	}

	if (ImGui::CollapsingHeader("Debug gizmos"))
	{
		ImGui::Checkbox("Show grid", &App->renderer->showGrid);
		ImGui::Checkbox("Show quadtree", &App->scene->showQuad);
		ImGui::NewLine();

	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::BulletText("Camera position: ( %f, %f, %f )", App->camera->camPos.x, App->camera->camPos.y, App->camera->camPos.z);
		ImGui::BulletText("Up directions: ( %f, %f, %f )", App->camera->up.x, App->camera->up.y, App->camera->up.z);
		ImGui::BulletText("Forward directions: ( %f, %f, %f )", App->camera->fwd.x, App->camera->fwd.y, App->camera->fwd.z);
		ImGui::BulletText("Side directions: ( %f, %f, %f )", App->camera->side.x, App->camera->side.y, App->camera->side.z);
		ImGui::NewLine();
		ImGui::InputFloat("Near Plane", &App->camera->frustum.nearPlaneDistance);
		ImGui::InputFloat("Far Plane", &App->camera->frustum.farPlaneDistance);
		ImGui::NewLine();
	

	}
	if (ImGui::CollapsingHeader("Window"))
	{
		int lastW = App->camera->screenWidth;
		int lastH = App->camera->screenHeight;
		int actualW = App->camera->screenWidth;
		int actualH = App->camera->screenHeight;


		ImGui::InputInt("Window width", &actualW, 20, 200);
		ImGui::InputInt("Window height", &actualH, 20, 200);

		if (actualW != lastW || actualH != lastH) {
			App->renderer->WindowResized(actualW, actualH);
			SDL_SetWindowSize(App->window->window, actualW, actualH);
		}

	}
	if (ImGui::CollapsingHeader("Information"))
	{
		ImGui::Text("Application Time = %d", SDL_GetTicks() / 1000);
		char* title = new char[50];
		updateFramerates();

		sprintf_s(title, 50, "Framerate %.1f", fps_log[logFPSIterator]);
		ImGui::PlotHistogram("", fps_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));

		sprintf_s(title, 50, "Milliseconds %.1f", ms_log[logMSIterator]);
		ImGui::PlotHistogram("", ms_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));

		sprintf_s(title, 50, "Memory used (MB)%.1f", memory_log[logMemoryIterator]);
		ImGui::PlotHistogram("", memory_log, 50, 0, title, 0.0f, 1000.f, ImVec2(350, 100));

		ImGui::Separator();
		ImGui::Text("Graphic card vendor: %s \n", glGetString(GL_VENDOR));
		ImGui::Text("Graphic card used: %s \n", glGetString(GL_RENDERER));
		ImGui::NewLine();
		ImGui::Text("OpenGL version: %s", glGetString(GL_VERSION));
		SDL_version version;
		SDL_GetVersion(&version);
		ImGui::Text("SDL version: %d.%d.%d \n", version.major, version.minor, version.patch);
		ImGui::Text("ImGui version: %s \n", ImGui::GetVersion());

	}

	ImGui::End();
	return UPDATE_CONTINUE;
}

update_status ModuleMenu::Inspector() {
	ImGui::SetNextWindowPos(ImVec2(App->camera->screenWidth - columnWidth, mainMenuSize.y));
	ImGui::SetNextWindowSize(ImVec2(columnWidth, App->camera->screenHeight - mainMenuSize.y)); 
	bool obert = true;

	GameObject* GO_act = App->scene->GO_selected;

	ImGui::Begin("Inspector", &obert, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	ImGui::Text(GO_act->name);
	ImGui::SameLine();
	ImGui::Text("| UID: %u", GO_act->UID);
	ImGui::Checkbox("Active", &GO_act->active);

	if (ImGui::CollapsingHeader("Transformation"),true)
	{
		if (ImGui::Button("Reset")) GO_act->transform->Reset();

		float variation = 15;	
		ImGui::Text("Position");
		ImGui::PushItemWidth(columnWidth / 4);
		ImGui::PushID("1");
		if (ImGui::SliderFloat("", &GO_act->transform->position.x, -variation, variation)) GO_act->transform->changed = true ;
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID("2");
		if (ImGui::SliderFloat("", &GO_act->transform->position.y, -variation, variation)) GO_act->transform->changed = true;
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID("3");
		if (ImGui::SliderFloat("", &GO_act->transform->position.z, -variation, variation)) GO_act->transform->changed = true;
		ImGui::PopID();
		ImGui::PopItemWidth();
	
		variation = 200;
		math::float3 rotationAux = GO_act->transform->eulerRot;
		rotationAux = RadToDeg(rotationAux);
		ImGui::Text("Rotation");
		ImGui::PushItemWidth(columnWidth / 4);
		ImGui::PushID("4");
		if (ImGui::SliderFloat("", &rotationAux.x, -variation, variation)) GO_act->transform->changed = true;
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID("5");
		if (ImGui::SliderFloat("", &rotationAux.y, -variation, variation)) GO_act->transform->changed = true;
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID("6");
		if (ImGui::SliderFloat("", &rotationAux.z, -variation, variation)) GO_act->transform->changed = true;
		ImGui::PopID();
		ImGui::PopItemWidth();
		rotationAux = DegToRad(rotationAux);
		GO_act->transform->eulerRot = rotationAux;
		GO_act->transform->rotation = Quat::FromEulerXYZ(rotationAux.x, rotationAux.y, rotationAux.z);

		variation = 2;
		ImGui::Text("Scale");
		ImGui::PushItemWidth(columnWidth / 4);
		ImGui::PushID("7");
		if (ImGui::SliderFloat("", &GO_act->transform->scale.x, -variation, variation)) GO_act->transform->changed = true;
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID("8");
		if (ImGui::SliderFloat("", &GO_act->transform->scale.y, -variation, variation)) GO_act->transform->changed = true;
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID("9");
		if (ImGui::SliderFloat("", &GO_act->transform->scale.z, -variation, variation)) GO_act->transform->changed = true;
		ImGui::PopID();
		ImGui::PopItemWidth();
		
	}

	std::vector<Component*> meshes = GO_act->GetComponents(MESH);
	for (int i = 0; i < meshes.size(); ++i) {
		ComponentMesh* meshact = (ComponentMesh*)meshes[i];

		ImGui::PushID(i);
		if (ImGui::CollapsingHeader("Mesh"), true)
		{
			ImGui::Text("Number of triangles: %i \n", meshact->mesh.numFaces);
			ImGui::PushID(i + 1);
			ImGui::Checkbox("Draw mesh", &(meshact->active));
			ImGui::PopID();			
		}
		ImGui::PopID();
	}

	if (ImGui::CollapsingHeader("Texture"))
	{
		std::vector<Component*> comps = GO_act->GetComponents(MESH);
		ComponentMesh* meshAux;
		char* titleTexture = new char[50];
		for (int i = 0; i < comps.size(); ++i) {		
			meshAux = (ComponentMesh*) comps[i];		
			sprintf(titleTexture, "Draw texture %i", i);
			ImGui::Checkbox(titleTexture, &meshAux->renderTexture);
		}	
	}

	
	ImGui::End();
	return UPDATE_CONTINUE;
}

update_status ModuleMenu::Hierarchy() {
	ImGui::SetNextWindowPos(ImVec2(App->camera->screenWidth - 2 * columnWidth, mainMenuSize.y));
	ImGui::SetNextWindowSize(ImVec2(columnWidth, (App->camera->screenHeight - mainMenuSize.y) / 2));
	bool obert = true;
	ImGui::Begin("Hierarchy", &obert);
	
	if (ImGui::Button("Save Scene")) {
		App->scene->SaveScene();
		char* b = new char[50];
		sprintf(b, "-- sceneDefault.tte saved --");
		App->menu->console.AddLog(b);
	}

	

	FillTree(App->scene->ROOT);
	

	ImGui::End();
	return UPDATE_CONTINUE;
}

update_status ModuleMenu::Update() {
	columnWidth = (App->camera->screenWidth - (App->camera->screenWidth * columnScreenRatio)) / 2;
	
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);

	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

	update_status ret = UPDATE_CONTINUE;

	if (showWindows) {
		ImGui::SetNextWindowPos(ImVec2(0, App->camera->screenHeight - consoleHeight));
		ImGui::SetNextWindowSize(ImVec2(App->camera->screenWidth - columnWidth * 2, consoleHeight));
		console.Draw("Console");

		if ((ret = MainBarMenu()) != UPDATE_CONTINUE) return ret;
		if ((ret = Inspector()) != UPDATE_CONTINUE) return ret;
		if ((ret = Configuration()) != UPDATE_CONTINUE) return ret;
		if ((ret = Hierarchy()) != UPDATE_CONTINUE) return ret;
	
		
	}
	
	return ret;
}

update_status ModuleMenu::PostUpdate() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}

bool ModuleMenu::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	delete fps_log;
	delete ms_log;
	return true;
}

