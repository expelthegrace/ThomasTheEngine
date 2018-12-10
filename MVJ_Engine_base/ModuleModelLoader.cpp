#include "ModuleModelLoader.h"
#include "GL/glew.h"
#include "SDL.h"
#include "ModuleMenu.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}


bool ModuleModelLoader::LoadBuffers(const aiScene* sceneActual, ComponentMesh* meshComp, int idMesh) {
	Mesh* mesh = &meshComp->mesh;
	const aiMesh* src_mesh = sceneActual->mMeshes[idMesh];

	mesh->type = VBO;
	mesh->numVertices = src_mesh->mNumVertices;
	mesh->numFaces = src_mesh->mNumFaces;

	unsigned* vboActual = &mesh->vbo;

	glGenBuffers(1, vboActual);
	glBindBuffer(GL_ARRAY_BUFFER, *vboActual);

	// Positions

	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2)*src_mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * src_mesh->mNumVertices, src_mesh->mVertices);

	math::float2* texture_coords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * src_mesh->mNumVertices,
		sizeof(float) * 2 * src_mesh->mNumVertices, GL_MAP_WRITE_BIT);
	for (unsigned i = 0; i < src_mesh->mNumVertices; ++i)
	{
		texture_coords[i] = math::float2(src_mesh->mTextureCoords[0][i].x, src_mesh->mTextureCoords[0][i].y);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned* iboActual = &mesh->ibo;

	glGenBuffers(1, iboActual);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *iboActual);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*src_mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
		sizeof(unsigned)*src_mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i < src_mesh->mNumFaces; ++i)
	{
		assert(src_mesh->mFaces[i].mNumIndices == 3);

		*(indices++) = src_mesh->mFaces[i].mIndices[0];
		*(indices++) = src_mesh->mFaces[i].mIndices[1];
		*(indices++) = src_mesh->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	int numMaterials = sceneActual->mNumMaterials;
	int index = meshComp->my_go->components.size()  - idMesh;
	ComponentMaterial* compMat = (ComponentMaterial*)meshComp->my_go->components[index - numMaterials + src_mesh->mMaterialIndex];

	mesh->materialIndex = compMat->material;
	mesh->numVertices = src_mesh->mNumVertices;
	mesh->numFaces = src_mesh->mNumFaces;
	mesh->numIndexesMesh = src_mesh->mNumFaces * 3;

	float3 * verticesAux = new float3[src_mesh->mNumVertices];

	for (int i = 0; i < src_mesh->mNumVertices; ++i) 
		verticesAux[i] = float3(src_mesh->mVertices[i].x, src_mesh->mVertices[i].y, src_mesh->mVertices[i].z);
	
	mesh->vertices = verticesAux;

	sprintf(b, ">Mesh loaded \n");
	App->menu->console.AddLog(b);
	sprintf(b, "Number of vertices: %u \n", mesh->numVertices);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of faces: %u \n", mesh->numFaces);
	App->menu->console.AddLog(b);

	
	return true;
}

void ModuleModelLoader::GenerateMesh(int idMesh, ComponentMesh * compMesh, const char* path) {

	const aiScene* sceneAct = aiImportFile(path, aiProcess_Triangulate);
	const char* errorMesage;

	sprintf(b, "Loading model with path: %s \n", path);
	App->menu->console.AddLog(b);

	if (scene == nullptr) {
		errorMesage = aiGetErrorString();
		sprintf(b, "Error loading model: %s", errorMesage);
		App->menu->console.AddLog(b);
		compMesh->mesh.numVertices = 0;
	}
	else LoadBuffers(sceneAct, compMesh, idMesh);


}

unsigned ModuleModelLoader::GenerateMaterial(int idMaterial, const char* path) {

	const aiScene* sceneAct = aiImportFile(path, aiProcess_Triangulate);

	const aiMaterial* src_material = sceneAct->mMaterials[idMaterial];
	unsigned dst_material;

	aiString file;
	aiTextureMapping mapping;
	unsigned uvindex = 0;

	if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
	{
		dst_material = App->textures->Load(file.C_Str(), false);
	}

	return dst_material;
}



bool ModuleModelLoader::LoadNewModel(char* path) {

	if (modelLoaded) {
		delete[] vbos;
		delete[] ibos;
		delete[] textures;
		delete[] materials;
		delete[] numVerticesMesh;
		delete[] numIndexesMesh;
	}

	numVertices = 0;
	numFaces = 0;

	scene = aiImportFile(path, aiProcess_Triangulate);
	const char* errorMesage;

	if (scene == nullptr) {
		errorMesage = aiGetErrorString();
		sprintf(b, "Error loading model: %s", errorMesage);
		App->menu->console.AddLog(b);
	}
	else {
		numMeshes = scene->mNumMeshes;

		vbos = new unsigned[numMeshes];
		ibos = new unsigned[numMeshes];
		textures = new unsigned[numMeshes];
		materials = new unsigned[numMeshes];
		numVerticesMesh = new unsigned[numMeshes];
		numIndexesMesh = new unsigned[numMeshes];


		//GenerateMeshes(scene);
		//GenerateMaterials(scene);
	}

	modelPosition = boundingBox->CenterPoint();
	modelScale = { 1,1,1 };
	modelRotation = { 0,0,0 };


	//Console data update

	sprintf(b, "------------------ Model loaded ------------------ \n");
	App->menu->console.AddLog(b);
	sprintf(b, "Model loaded with path: %s \n", path);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of meshes: %u \n", numMeshes);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of vertices: %u \n", numVertices);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of faces: %u \n", numFaces);
	App->menu->console.AddLog(b);
	sprintf(b, "Bounding box max: (%f, %f, %f) \n", boundingBox->MaxX(), boundingBox->MaxY(), boundingBox->MaxZ());
	App->menu->console.AddLog(b);
	sprintf(b, "Bounding box min: (%f, %f, %f) \n", boundingBox->MinX(), boundingBox->MinY(), boundingBox->MinZ());
	App->menu->console.AddLog(b);
	sprintf(b, "Center point: (%f, %f, %f) \n", boundingBox->CenterPoint().x, boundingBox->CenterPoint().y, boundingBox->CenterPoint().z);
	App->menu->console.AddLog(b);

	App->camera->FocusModel();

	modelLoaded = true;
	return true;

}



bool ModuleModelLoader::Init() {
	modelLoaded = false;
	//LoadNewModel("BakerHouse.fbx");

	return true;
}

bool ModuleModelLoader::CleanUp() {
	/*delete[] vbos;
	delete[] ibos;
	delete[] textures;
	delete[] materials;
	delete[] numVerticesMesh;
	delete[] numIndexesMesh;
	*/
	aiDetachAllLogStreams();
	return true;
}
