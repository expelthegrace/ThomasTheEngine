#ifndef _MESHCOMP_
#define _MESHCOMP_

#include "Component.h"
#include "MathGeoLib.h"
#include <vector>

class GameObject;

struct Mesh {

	unsigned vbo;
	unsigned ibo;
	unsigned numIndexesMesh;
	unsigned materialIndex;
	unsigned numVertices;
	unsigned numFaces;
	float3* vertices;


};

class ComponentMesh :
	public Component
{
public:
	ComponentMesh( GameObject* my_go);
	ComponentMesh(GameObject* my_go, const Mesh& mesh, float3* vertices);
	~ComponentMesh();

public:
	Mesh mesh;
	bool avaliable;
	bool renderTexture = true;


};

#endif

