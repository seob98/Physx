#pragma once

#include "framework.h"

enum class PhysicsCombineMode
{
	Average = PxCombineMode::eAVERAGE,
	Min = PxCombineMode::eMIN,
	Multiply = PxCombineMode::eMULTIPLY,
	Max = PxCombineMode::eMAX,
};

class Collider
{
	friend class RigidBody;

public:
	void Init();

protected:
	virtual PxGeometryHolder CreateGeometry() = 0;

protected:
	PxMaterial* m_material = nullptr;
	PxShape* m_shape = nullptr;
};

