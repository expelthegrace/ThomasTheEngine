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
	Quadtree(float3 min, float3 max, int bucket, int maxLvls);
	bool Insert(GameObject* go);
	bool Intersects(GameObject* go);
	void Divide();
	void Draw();
	void CollectIntersections(std::vector<GameObject*>& GOcollisioned, const GameObject* GO);
	void Clear();

	~Quadtree();


public:
	int bucket = 0;
	node nodeType = LEAF;

private:
	std::list<GameObject*> gameObjects;
	std::list<Quadtree*> branches;
	AABB* boundaries;
	bool divided = false;
	int maxDepth;

	char* b = new char[50];
};

#endif