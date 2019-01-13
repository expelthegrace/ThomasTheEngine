#include "ModuleModelLoader.h"
#include "GL/glew.h"
#include "SDL.h"
#include "ModuleMenu.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "GameObject.h"

ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}


bool ModuleModelLoader::LoadBuffers(GameObject*  GO, const aiScene* sceneActual, ComponentMesh* meshComp, int idMesh) {
	Mesh* mesh = &meshComp->mesh;
	const aiMesh* src_mesh = sceneActual->mMeshes[idMesh];

	// copies to assign to the mesh
	float3 * verticesAux = new float3[src_mesh->mNumVertices];
	float3 * indicesAux = new float3[src_mesh->mNumFaces];
	float3 * normalsAux = new float3[src_mesh->mNumVertices];

	mesh->type = VBO;
	mesh->numVertices = src_mesh->mNumVertices;
	mesh->numFaces = src_mesh->mNumFaces;

	unsigned* vboActual = &mesh->vbo;

	glGenBuffers(1, vboActual);
	glBindBuffer(GL_ARRAY_BUFFER, *vboActual);

	// Vertices & texture coords & normals
	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2 + sizeof(float) * 3)*src_mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * src_mesh->mNumVertices, src_mesh->mVertices);

	math::float2* texture_coords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * src_mesh->mNumVertices,
		sizeof(float) * 2 * src_mesh->mNumVertices, GL_MAP_WRITE_BIT);
	for (unsigned i = 0; i < src_mesh->mNumVertices; ++i)
	{
		texture_coords[i] = math::float2(src_mesh->mTextureCoords[0][i].x, src_mesh->mTextureCoords[0][i].y);
	}

	mesh->hasNormals = src_mesh->HasNormals();

	if (mesh->hasNormals) glBufferSubData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2 )*src_mesh->mNumVertices, sizeof(float) * 3 * src_mesh->mNumVertices, src_mesh->mNormals);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	for (int i = 0; i < src_mesh->mNumVertices; ++i) {
		verticesAux[i] = float3(src_mesh->mVertices[i].x, src_mesh->mVertices[i].y, src_mesh->mVertices[i].z);
		if (src_mesh->HasNormals()) normalsAux[i] = float3(src_mesh->mNormals[i].x, src_mesh->mNormals[i].y, src_mesh->mNormals[i].z);
	}

	// indices
	unsigned* iboActual = &mesh->ibo;

	glGenBuffers(1, iboActual);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *iboActual);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*src_mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
		sizeof(unsigned)*src_mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

	

	for (unsigned i = 0; i < src_mesh->mNumFaces; ++i)
	{
		assert(src_mesh->mFaces[i].mNumIndices == 3);

		indicesAux[i] = float3(src_mesh->mFaces[i].mIndices[0], src_mesh->mFaces[i].mIndices[1], src_mesh->mFaces[i].mIndices[2]);

		*(indices++) = src_mesh->mFaces[i].mIndices[0];
		*(indices++) = src_mesh->mFaces[i].mIndices[1];
		*(indices++) = src_mesh->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//int numMaterials = sceneActual->mNumMaterials;
	//int index = meshComp->my_go->components.size()  - idMesh;
	//ComponentMaterial* compMat = (ComponentMaterial*)meshComp->my_go->components[index - numMaterials + src_mesh->mMaterialIndex];
	//ComponentMaterial* compMat = GO->material;

	mesh->numTexCoords = src_mesh->mNumVertices;
	//mesh->texture = compMat->texture; // change
	mesh->numVertices = src_mesh->mNumVertices;
	mesh->numFaces = src_mesh->mNumFaces;
	mesh->numIndexesMesh = src_mesh->mNumFaces * 3;
	
	mesh->vertices = verticesAux;
	mesh->indices = indicesAux;
	mesh->normals = normalsAux;

	sprintf(b, ">Mesh loaded \n");
	App->menu->console.AddLog(b);
	sprintf(b, "Number of vertices: %u \n", mesh->numVertices);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of faces: %u \n", mesh->numFaces);
	App->menu->console.AddLog(b);
	sprintf(b, "Has normals: %i \n", mesh->hasNormals);
	App->menu->console.AddLog(b);

	
	return true;
}

/** Takes a mesh and loads on it all the mesh information **/
void ModuleModelLoader::GenerateMesh(GameObject* GO, int idMesh, ComponentMesh * compMesh, const char* path) {

	const aiScene* sceneAct = aiImportFile(path, aiProcess_Triangulate);
	const char* errorMesage;

	sprintf(b, "Loading model with path: %s \n", path);
	App->menu->console.AddLog(b);

	if (sceneAct == nullptr) {
		errorMesage = aiGetErrorString();
		sprintf(b, "Error loading model: %s", errorMesage);
		App->menu->console.AddLog(b);
		compMesh->mesh.numVertices = 0;
	}
	else LoadBuffers(GO, sceneAct, compMesh, idMesh);


}

int ModuleModelLoader::GenerateMaterial(int idMaterial, const char* path) {

	const aiScene* sceneAct = aiImportFile(path, aiProcess_Triangulate);

	int indexMaterial = sceneAct->mMeshes[idMaterial]->mMaterialIndex;
	const aiMaterial* src_material = sceneAct->mMaterials[indexMaterial];
	unsigned dst_material = 0;

	aiString file;
	aiTextureMapping mapping;
	unsigned uvindex = 0;

	if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
	{
		dst_material = App->textures->Load(file.C_Str(), false);
	}
	else return -1;
	

	return dst_material;
}




bool ModuleModelLoader::Init() {

	return true;
}

bool ModuleModelLoader::CleanUp() {
	
	aiDetachAllLogStreams();
	return true;
}
