#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum type_comp
{
	CAMERA = 1,
	MESH,
	MATERIAL,
	TRANSFORM
};

// Configuration -----------
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 960
#define FULLSCREEN false
#define BORDERLESS false
#define RESIZABLE_WINDOW true
#define VSYNC true
#define TITLE "Thomas The Engine"
#define GLSL_VERSION "#version 330"