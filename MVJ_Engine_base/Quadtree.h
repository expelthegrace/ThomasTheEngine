#ifndef _QUADTREE_
#define _QUADTREE_

#include <list>
#include <vector>
#include "MathGeoLib.h"

class GameObject;

enum node {
	TREE = 0,
	LEAF
};

class Quadtree
{
public:
	Quadtree(Quadtree*  parent, float3 min, float3 max, int bucket, int maxLvls);
	bool Insert(GameObject* go);
	void CollectIntersections(std::vector<GameObject*>& GOcollisioned, const GameObject* GO);
	void Clear();
		
	Quadtree* Find(const GameObject* GO);
	void MoveGO(GameObject* GO, Quadtree * lastQT);	
	void Remove(GameObject* GO);
	void RemoveAndMerge(GameObject* GO);

	void Draw();

	~Quadtree();

private:
	bool Intersects(const GameObject* go);
	void Divide();
	bool Collisions(const GameObject* GO);
	void Merge();
	bool IsBucketFree();

public:
	int bucket = 0;
	node nodeType = LEAF;

private:
	Quadtree* parent = nullptr;
	std::list<GameObject*> gameObjects;
	std::list<Quadtree*> branches;
	AABB* boundaries;
	bool divided = false;
	int maxDepth;

	char* b = new char[50];
};

#endif