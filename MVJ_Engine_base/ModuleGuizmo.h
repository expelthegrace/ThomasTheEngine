#ifndef _MODULEGUIZMO_
#define _MODULEGUIZMO_

#include "Module.h"

class ModuleGuizmo :
	public Module
{
public:
	ModuleGuizmo();
	~ModuleGuizmo();

	update_status PreUpdate() override;

};

#endif