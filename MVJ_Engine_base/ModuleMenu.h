#ifndef __ModuleMenu_h__
#define __ModuleMenu_h__
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "windows.h"
#include "psapi.h"

class GameObject;

class ModuleMenu :
	public Module
{
public:
	ModuleMenu();
	~ModuleMenu();

	bool            Init();
	update_status   PreUpdate();
	update_status   Update();
	update_status   PostUpdate();
	bool            CleanUp();

	void DrawCameras();
	void DrawEditorCamera();
	void DrawMainCamera();
	update_status MainBarMenu();
	update_status Configuration();
	update_status Inspector();
	update_status Hierarchy();

	void FillTree(GameObject* gameobject);

	struct ExampleAppLog
	{
		ImGuiTextBuffer     Buf;
		bool                ScrollToBottom;

		void    Clear() { Buf.clear(); }

		void    AddLog(const char* fmt, ...) //IM_PRINTFARGS(2)
		{
			va_list args;
			va_start(args, fmt);
			Buf.appendf(fmt, args);
			va_end(args);
			ScrollToBottom = true;
		}

		void    Draw(const char* title, bool* p_opened = NULL)
		{
			ImGui::Begin(title, p_opened);
			ImGui::TextUnformatted(Buf.begin());
			if (ScrollToBottom)
				ImGui::SetScrollHere(1.0f);
			ScrollToBottom = false;
			ImGui::End();
		}
	};

public:
	//variables
	int frames;
	double lastFrameTime;
	double lastSecondTime;
	int logMSIterator;
	int logFPSIterator;
	int logMemoryIterator;
	float* fps_log;
	float* ms_log;
	float* memory_log;
	ExampleAppLog console;
	
	PROCESS_MEMORY_COUNTERS pmc;
	MEMORYSTATUSEX memInfo;

	char* b;
	bool showWindows;
	bool fovChanged = false;
	int columnWidth;
	int consoleHeight;

private:
	void updateFramerates();
	ImVec2 mainMenuSize;
	
	float columnScreenRatio;

	bool cameraInitUpdated = false;

};

#endif /* __ModuleMenu_h__ */
