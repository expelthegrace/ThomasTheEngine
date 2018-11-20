#include "ComponentTransform.h"


ComponentTransform::ComponentTransform()
{
	scale = { 1.f,1.f,1.f };
	position = { 10.f,5.f,2.f };
	rotation = Quat::FromEulerXYZ(0, 0, 0);
}


ComponentTransform::~ComponentTransform()
{
}
