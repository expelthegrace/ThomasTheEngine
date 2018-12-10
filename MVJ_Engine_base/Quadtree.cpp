#include "Quadtree.h"
#include "GameObject.h"
#include "ComponentBB.h"
#include "debugdraw.h"
#include "ModuleMenu.h"
#include "Application.h"

Quadtree::Quadtree(float3 min, float3 max, int bucket, int maxLevels)
{
	
	this->bucket = bucket;
	boundaries = new AABB(min, max);
	maxDepth = maxLevels;

}

bool Quadtree::Intersects(GameObject* go) {
	if (!go->BB->Aabb->IsFinite()) return false;

	return boundaries->Contains(go->BB->Aabb->CenterPoint());
	//return (go->BB->Aabb->Intersects(*boundaries));
}


void Quadtree::Divide() {
	nodeType = TREE;

	float3 newMin = { boundaries->MinX(), boundaries->MinY(), (boundaries->MinZ() + boundaries->MaxZ()) / 2 };
	float3 newMax = { (boundaries->MinX() + boundaries->MaxX()) / 2, boundaries->MaxY(), boundaries->MaxZ() };
	Quadtree* newQuad = new Quadtree(newMin, newMax, bucket, maxDepth - 1);
	branches.push_back(newQuad);

	newMin = { (boundaries->MinX() + boundaries->MaxX()) / 2, boundaries->MinY(), (boundaries->MinZ() + boundaries->MaxZ()) / 2 };
	newMax = boundaries->maxPoint;
	newQuad = new Quadtree(newMin, newMax, bucket, maxDepth - 1);
	branches.push_back(newQuad);

	newMin = boundaries->minPoint;
	newMax = { (boundaries->MinX() + boundaries->MaxX()) / 2, boundaries->MaxY(), (boundaries->MinZ() + boundaries->MaxZ()) / 2 };
	newQuad = new Quadtree(newMin, newMax, bucket, maxDepth - 1);
	branches.push_back(newQuad);

	newMin = { (boundaries->MinX() + boundaries->MaxX()) / 2, boundaries->MinY(), boundaries->MinZ() };
	newMax = { boundaries->MaxX(), boundaries->MaxY(), (boundaries->MinZ() + boundaries->MaxZ()) / 2 };
	newQuad = new Quadtree(newMin, newMax, bucket, maxDepth - 1);
	branches.push_back(newQuad);

	
		
	for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) {
		for (std::list<GameObject*>::iterator itGO = gameObjects.begin(); itGO != gameObjects.end(); ++itGO) if ((*it)->Insert(*itGO)) {
			gameObjects.clear();
			return;
		}
	}

	gameObjects.clear();
}

bool Quadtree::Insert(GameObject * go) {

	if (!this->Intersects(go)) return false;

	if (nodeType == LEAF) {
		if (gameObjects.size() < bucket) gameObjects.push_back(go);
		else if (maxDepth > 0) {
			Divide();

			for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) {
				if ((*it)->Insert(go)) return true;
			}
		}
		else {		
			sprintf(b, "ERROR: Quadtree max level reached, game object %s not added to the tree. Possible infinte loop", go->name);
			App->menu->console.AddLog(b);

			return false;
		}
	}
	else for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) if ((*it)->Insert(go)) return true;

	return false;
}

void Quadtree::CollectIntersections(std::vector<GameObject*>& GOcollisioned, const GameObject* GO) {

	if (GO->BB->Aabb->IsFinite() && boundaries->Intersects(*(GO->BB->Aabb)) ) {

		if (this->nodeType == LEAF) {
			for (std::list<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) 
				GOcollisioned.push_back(*it);
			
		}
		else {
			for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) 
				(*it)->CollectIntersections(GOcollisioned,GO);
		}
	}
}


void Quadtree::Draw() {
	const ddVec3 boxColor = { 0.4f, 0.4f, 0.8f };
	dd::aabb(boundaries->minPoint, boundaries->maxPoint, boxColor);

	for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) (*it)->Draw();
}

void Quadtree::Clear() {
	delete b;
	b = nullptr;

	if (nodeType == TREE) {
		for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) {
			delete *it;
			*it = nullptr;
			branches.clear();
		}
	}
	else {
		for (std::list<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) *it = nullptr;
		gameObjects.clear();
	}
}

Quadtree::~Quadtree()
{
	
}
