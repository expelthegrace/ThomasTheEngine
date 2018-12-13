#include "Quadtree.h"
#include "GameObject.h"
#include "ComponentBB.h"
#include "debugdraw.h"
#include "ModuleMenu.h"
#include "Application.h"
#include "ModuleScene.h"

Quadtree::Quadtree(Quadtree* parent, float3 min, float3 max, int bucket, int maxLevels)
{
	this->parent = parent;
	this->bucket = bucket;
	boundaries = new AABB(float3 (min.x, -5.0f, min.z), float3( max.x , 5.0f, max.z));
	maxDepth = maxLevels;

}

bool Quadtree::Intersects(const GameObject* go) {
	if (!go->BB->Aabb->IsFinite()) return false;
	

	return (boundaries->MinX() <= go->BB->Aabb->CenterPoint().x &&
		boundaries->MinZ() <= go->BB->Aabb->CenterPoint().z &&
		boundaries->MaxX() >= go->BB->Aabb->CenterPoint().x &&
		boundaries->MaxZ() >= go->BB->Aabb->CenterPoint().z);

	//return boundaries->Contains(go->BB->Aabb->CenterPoint());
	//return (go->BB->Aabb->Intersects(*boundaries));
}

Quadtree* Quadtree::Find(const GameObject* GO) {

	if (!Intersects(GO)) return nullptr;

	if (this->nodeType == LEAF) {
		for (std::list<GameObject*>::iterator itGO = gameObjects.begin(); itGO != gameObjects.end(); ++itGO)
			if ((*itGO)->UID == GO->UID) return this;
	}
	else {
		for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) {
			Quadtree* ret = (*it)->Find(GO);
			if (ret != nullptr) return ret;
		}		
	}
	return nullptr;
}

void Quadtree::Divide() {
	nodeType = TREE;

	float3 newMin = { boundaries->MinX(), boundaries->MinY(), (boundaries->MinZ() + boundaries->MaxZ()) / 2 };
	float3 newMax = { (boundaries->MinX() + boundaries->MaxX()) / 2, boundaries->MaxY(), boundaries->MaxZ() };
	Quadtree* newQuad = new Quadtree(this, newMin, newMax, bucket, maxDepth - 1);
	branches.push_back(newQuad);

	newMin = { (boundaries->MinX() + boundaries->MaxX()) / 2, boundaries->MinY(), (boundaries->MinZ() + boundaries->MaxZ()) / 2 };
	newMax = boundaries->maxPoint;
	newQuad = new Quadtree(this, newMin, newMax, bucket, maxDepth - 1);
	branches.push_back(newQuad);

	newMin = boundaries->minPoint;
	newMax = { (boundaries->MinX() + boundaries->MaxX()) / 2, boundaries->MaxY(), (boundaries->MinZ() + boundaries->MaxZ()) / 2 };
	newQuad = new Quadtree(this, newMin, newMax, bucket, maxDepth - 1);
	branches.push_back(newQuad);

	newMin = { (boundaries->MinX() + boundaries->MaxX()) / 2, boundaries->MinY(), boundaries->MinZ() };
	newMax = { boundaries->MaxX(), boundaries->MaxY(), (boundaries->MinZ() + boundaries->MaxZ()) / 2 };
	newQuad = new Quadtree(this, newMin, newMax, bucket, maxDepth - 1);
	branches.push_back(newQuad);

		
	for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) {
		for (std::list<GameObject*>::iterator itGO = gameObjects.begin(); itGO != gameObjects.end(); ++itGO) if ((*it)->Insert(*itGO)) {
			gameObjects.clear();
			return;
		}
	}

	gameObjects.clear();
}


bool Quadtree::IsBucketFree() {

	int bucketTotal = 0;
	for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) {
		if ((*it)->nodeType == TREE) return false;
		bucketTotal += (*it)->gameObjects.size();
	}
	return bucketTotal <= bucket;
}

void Quadtree::Merge() {

	if (IsBucketFree()) {

		gameObjects.clear();

		for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) {
			for (std::list<GameObject*>::iterator itGO = (*it)->gameObjects.begin(); itGO != (*it)->gameObjects.end(); ++itGO)
				gameObjects.push_back(*itGO);
			(*it)->Clear();
			*it = nullptr;			
		}
		branches.clear();
		nodeType = LEAF;
	}
	int a = 3;
}

void Quadtree::Remove(GameObject* GO) {
	Quadtree* currentQT = App->scene->quadTree->Find(GO);

	if (currentQT != nullptr) currentQT->gameObjects.remove(GO);
	currentQT = nullptr;
}

void Quadtree::MoveGO(GameObject* GOact, Quadtree * lastQT) {

	//Quadtree* currentQT = Find(&lastFrameGO);
	if (lastQT != nullptr) lastQT->gameObjects.remove(GOact);

	App->scene->quadTree->Insert(GOact);	

	
	if (lastQT != nullptr) {
		if (lastQT->parent != nullptr) lastQT->parent->Merge();
	}
	
	//currentQT = nullptr;
}

bool Quadtree::Insert(GameObject * go) {

	if (!go->BB->Aabb->IsFinite()) return false;
	if (!Intersects(go)) return false;

	if (nodeType == LEAF) {
		if (gameObjects.size() < bucket) gameObjects.push_back(go);
		else if (maxDepth > 0) {
			Divide();

			for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) {
				if ((*it)->Insert(go)) return true;
			}
		}
		else {		
			//sprintf(b, "ERROR: Quadtree max level reached, game object %s not added to the tree. Possible infinte loop", go->name);
		//	App->menu->console.AddLog(b);

			return false;
		}
	}
	else for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) if ((*it)->Insert(go)) return true;

	return false;
}

bool Quadtree::Collisions(const GameObject* go) {
	return (boundaries->MinX() <= go->BB->Aabb->MinX() &&
		boundaries->MinZ() <= go->BB->Aabb->MinZ() &&
		boundaries->MaxX() >= go->BB->Aabb->MaxX() &&
		boundaries->MaxZ() >= go->BB->Aabb->MaxZ());
}

void Quadtree::CollectIntersections(std::vector<GameObject*>& GOcollisioned, const GameObject* GO) {

	if (GO->BB->Aabb->IsFinite() && Collisions(GO) ) {

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
	/*
	if (true) {

		LOG("> %i (%i)", this->maxDepth, nodeType);

		if (nodeType == LEAF) {
			for (std::list<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) {
				LOG("- %s", (*it)->name);
			}
		}

	}
	*/
	const ddVec3 boxColor = { 0.4f, 0.4f, 0.8f };
	dd::aabb(boundaries->minPoint, boundaries->maxPoint, boxColor);

	for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) (*it)->Draw();
}

void Quadtree::Clear() {
	

	if (nodeType == TREE) {
		for (std::list<Quadtree*>::iterator it = branches.begin(); it != branches.end(); ++it) {
			(*it)->Clear();			
			*it = nullptr;
			branches.clear();
		}
	}
	else {
		for (std::list<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) *it = nullptr;
		gameObjects.clear();
	}

	delete boundaries;
	boundaries = nullptr;

	//delete b;
	//b = nullptr;
}

Quadtree::~Quadtree()
{
	
}
