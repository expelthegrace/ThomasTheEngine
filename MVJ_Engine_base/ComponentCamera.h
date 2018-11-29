#ifndef _COMPCAMERA_
#define _COMPCAMERA_

#include "Globals.h"
#include "Component.h"

class ComponentCamera :
	public Component
{
public:
	ComponentCamera();
	~ComponentCamera();


public:
	unsigned fbo = 0;
	unsigned fb_depth = 0;
	unsigned fb_tex = 0;
	unsigned fb_width = 0;
	unsigned fb_height = 0;
	
};

#endif