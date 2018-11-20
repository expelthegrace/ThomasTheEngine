#include "ComponentTransform.h"


ComponentTransform::ComponentTransform()
{
	scale = { 1.f,1.f,1.f };
	position = { 0.f,0.f,0.f };
	rotation = { 0.f, 0.f, 0.f };
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Reset() {
	scale = { 1.f,1.f,1.f };
	position = { 0.f,0.f,0.f };
	rotation = { 0.f, 0.f, 0.f };
}