#ifndef _MESHCOMP_
#define _MESHCOMP_

#include "Component.h"

class GameObject;

struct Mesh {

	unsigned vbo;
	unsigned ibo;
	unsigned numIndexesMesh;
	unsigned materialIndex;
	unsigned numVertices;
	unsigned numFaces;

};

class ComponentMesh :
	public Component
{
public:
	ComponentMesh( GameObject* my_go);
	ComponentMesh(GameObject* my_go, const Mesh& mesh);
	~ComponentMesh();

public:
	Mesh mesh;
	bool avaliable;


};

#endif

