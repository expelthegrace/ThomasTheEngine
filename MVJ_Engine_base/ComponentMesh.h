#ifndef _MESHCOMP_
#define _MESHCOMP_

#include "Component.h"
#include "MathGeoLib.h"
#include <vector>
#include <string>

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
	//unsigned texture;
	unsigned numVertices;
	unsigned numFaces;
	unsigned vao;
	unsigned numTexCoords;
	unsigned* buffer = new unsigned[10];
	bool hasNormals = false;
	float3 * vertices;
	float3 * indices;
	float3 * normals;


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
	std::string path = "";
	int idMesh;


private:
};

#endif

