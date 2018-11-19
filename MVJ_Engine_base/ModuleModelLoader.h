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

	//carlos
	void            GenerateMeshes(const aiScene* scene);
	void            GenerateMaterials(const aiScene* scene);

	Mesh GenerateMesh(int idMesh, const char* path);

	bool LoadBuffers(const aiScene* scene, Mesh& mesh, int idMesh);

public:


	unsigned* vbos;
	unsigned* ibos;
	unsigned* textures;
	unsigned* materials;
	unsigned*  numVerticesMesh;
	unsigned*  numIndexesMesh;

	float3 modelRotation, modelScale, modelPosition;

	char* b = new char[50];

	unsigned numMeshes;
	unsigned numVertices;
	unsigned numFaces;

	bool modelLoaded;

	float minY, maxY, minX, maxX, minZ, maxZ;

	AABB* boundingBox;

	const aiScene* scene;

};

#endif