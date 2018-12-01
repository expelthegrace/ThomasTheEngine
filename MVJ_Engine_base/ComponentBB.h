#ifndef _COMPBB_
#define _COMPBB_

#include "Component.h"
#include <vector>
#include "MathGeoLib.h"

class ComponentMesh;

class ComponentBB :
	public Component
{
public:
	ComponentBB(GameObject* my_go);
	~ComponentBB();

	void UpdateBB();
	update_status Update() override;
	void SetAABB(std::vector<ComponentMesh*>* meshes);


public:
	AABB* Aabb;

	float3* cornersAABB;
};

#endif