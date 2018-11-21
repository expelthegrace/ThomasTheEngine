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

	columnScreenRatio = 6.f / 9.f;

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

update_status ModuleMenu::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
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
				ShellExecute(NULL, "open", "https://github.com/expelthegrace/Engine_1st_Assign", NULL, NULL, SW_SHOWNORMAL);
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

	ImGui::SetNextWindowPos(ImVec2(App->camera->screenWidth - columnWidth, (App->camera->screenHeight - mainMenuSize.y) / 2 + mainMenuSize.y));
	ImGui::SetNextWindowSize(ImVec2(columnWidth, (App->camera->screenHeight - mainMenuSize.y) / 2));
	ImGui::Begin("Configuration", &obert);

	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse position: %i , %i", App->input->mouse_position.x, App->input->mouse_position.y);
		ImGui::Text("Mousewheel force: %i", App->input->mouseWheel);

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
		ImGui::Checkbox("Show grid", &App->renderer->showGrid);


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
	ImGui::SetNextWindowSize(ImVec2(columnWidth, App->camera->screenHeight - mainMenuSize.y)); //(App->camera->screenHeight - mainMenuSize.y) / 2)
	bool obert = true;
	ImGui::Begin("Inspector", &obert);

	if (ImGui::CollapsingHeader("Transformation"))
	{
		float v3pos[3] = { App->modelLoader->modelPosition.x,App->modelLoader->modelPosition.y,App->modelLoader->modelPosition.z };
		ImGui::InputFloat3("Position", v3pos);
		float v3rot[3] = { App->modelLoader->modelRotation.x,App->modelLoader->modelRotation.y,App->modelLoader->modelRotation.z };
		ImGui::InputFloat3("Rotation", v3rot);
		float v3scale[3] = { App->modelLoader->modelScale.x,App->modelLoader->modelScale.y,App->modelLoader->modelScale.z };
		ImGui::InputFloat3("Scale", v3scale);

	}
	if (ImGui::CollapsingHeader("Geometry"))
	{
		ImGui::Text("Number of triangles: %i \n", App->modelLoader->numFaces);
	}
	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Checkbox("Draw texture", &App->renderer->renderTexture);
	}

	ImGui::End();
	return UPDATE_CONTINUE;
}

update_status ModuleMenu::Hierarchy() {
	ImGui::SetNextWindowPos(ImVec2(App->camera->screenWidth - 2 * columnWidth, mainMenuSize.y));
	ImGui::SetNextWindowSize(ImVec2(columnWidth, App->camera->screenHeight - mainMenuSize.y));
	bool obert = true;
	ImGui::Begin("Hierarchy", &obert);

	

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

