#ifndef _MESHMANAGER_
#define _MESHMANAGER_

#include "Globals.h"
#include <string>
#include "MathGeoLib.h"

struct aiMesh;
class ComponentMesh;
class Resource;

class MeshManager
{
public:
	MeshManager();
	~MeshManager();

	bool importRMesh(aiMesh* mesh, uint UID, std::string& path, float3 color);
	bool saveMesh(ComponentMesh* mesh, uint UID, std::string& newpath);

private:
	char * runPath;
};

#endif