#ifndef _MODELLOADER_
#define _MODELLOADER_
#include "Module.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include "MathGeoLib.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

class GameObject;

class ModuleModelLoader :
	public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool            Init();
	//update_status   Update();
	bool            CleanUp();


	bool LoadNewModel(char* path);

	void GenerateMesh(GameObject* GO, int idMesh, ComponentMesh* compMesh, const char* path);
	int GenerateMaterial(int idMaterial, const char* path);

	bool LoadBuffers(GameObject*  GO, const aiScene* scene, ComponentMesh* meshComp, int idMesh);

public:
	char* b = new char[50];

	/*unsigned* vbos;
	unsigned* ibos;
	unsigned* textures;
	unsigned* materials;
	unsigned*  numVerticesMesh;
	unsigned*  numIndexesMesh;

	float3 modelRotation, modelScale, modelPosition;

	

	unsigned numMeshes;
	unsigned numVertices;
	unsigned numFaces;

	bool modelLoaded;

	float minY, maxY, minX, maxX, minZ, maxZ;

	AABB* boundingBox;

	const aiScene* scene;*/

};

#endif