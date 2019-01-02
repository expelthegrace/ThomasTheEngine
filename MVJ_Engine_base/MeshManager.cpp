#include "MeshManager.h"
#include "SDL.h"
#include "SDL_rwops.h"
#include "ComponentMesh.h"

MeshManager::MeshManager()
{
	runPath = SDL_GetBasePath();
	
}


MeshManager::~MeshManager()
{
	delete runPath;
}

bool MeshManager::saveMesh(ComponentMesh* meshComp, uint UID, std::string& newpath)
{
	bool ret = true;
	Mesh* mesh = &(meshComp->mesh);

	//	Vertices | Indices | Normals(vertices) | Texture coords (verties*2)
	uint ranges[4] = { mesh->numVertices, mesh->numIndexesMesh, mesh->numTexCoords };

	//Total size of the buffer

	// v wrong, both mesh.indeces and mesh.vertices are float3 arrays!!!!!!!!!!!!!!!
	uint size = sizeof(ranges) + sizeof(float)*mesh->numVertices * 3 + sizeof(uint)*mesh->numIndexesMesh + sizeof(float)*mesh->numTexCoords * 2;
	size += sizeof(float) * 3;
	char* buffer = new char[size];
	char* cursor = buffer;

	//Store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	//Store vertices
	bytes = sizeof(float)*mesh->numVertices * 3;
	memcpy(cursor, mesh->vertices, bytes);
	cursor += bytes;

	//Store indices
	bytes = sizeof(uint)*mesh->numIndexesMesh;
	memcpy(cursor, mesh->indices, bytes);
	cursor += bytes;

	//Store normals
	/*bytes = sizeof(float)*mesh->num_normals * 3;
	memcpy(cursor, mesh->normals, bytes);
	cursor += bytes;*/

	//Store tex_coords
	//bytes = sizeof(float)*mesh->numTexCoords * 2;
	//memcpy(cursor, mesh->texCoords, bytes);
	//cursor += bytes;

	////Store Color
	//float color[3] = { mesh->Color.x,mesh->Color.y,mesh->Color.z };
	//bytes = sizeof(float) * 3;
	//memcpy(cursor, color, bytes);

	//App->fileSystem->writeFile((MESHES_FOLDER + std::to_string(UID) + MESH_EXTENSION).c_str(), buffer, size, true);
	//RELEASE_ARRAY(buffer);

	////TODO: the name should be the UID of the resource
	//newpath = MESHES_FOLDER + std::to_string(UID) + MESH_EXTENSION;

	return ret;
}
