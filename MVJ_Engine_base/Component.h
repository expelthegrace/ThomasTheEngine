#ifndef _COMPONENT_
#define _COMPONENT_

#include "Globals.h"
#include "GameObject.h"

class GameObject;
struct JSON_Value;

class Component
{
public:
	Component() {
	
	}

	virtual ~Component() {

	}

	virtual void Enable() {}
	virtual void Disable() {}
	virtual update_status Update() { 
		return UPDATE_CONTINUE; 
	}

	virtual void Save(JSON_Value* componentsJSON) {}

	virtual void Load(JSON_Value* commponentJSON) {}

public:
	unsigned UID;
	type_comp type;
	bool active = true;
	GameObject* my_go;

};

#endif

