#ifndef _MESHCOMP_
#define _MESHCOMP_

#include "Component.h"
#include "MathGeoLib.h"
#include <vector>

class GameObject;

enum meshType {
	VBO,
	VAO
};

struct Mesh {

	meshType type;
	unsigned vbo;
	unsigned ibo;
	unsigned numIndexesMesh;
	unsigned materialIndex;
	unsigned numVertices;
	unsigned numFaces;
	unsigned vao;
	unsigned* buffer = new unsigned[10];
	float3* vertices;


};

class ComponentMesh :
	public Component
{
public:
	ComponentMesh( GameObject* my_go);
	ComponentMesh(GameObject* my_go, const Mesh& mesh, float3* vertices);
	~ComponentMesh();

	void Save(JSON_Value* componentsJSON) override;
	void Load(JSON_Value* componentJSON) override;


public:
	Mesh mesh;
	bool avaliable;
	bool renderTexture = true;
	char* path = nullptr;


private:
};

#endif

