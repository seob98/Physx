#pragma once
#include "Client_Defines.h"

class Collider;

class BoxCollider : public Collider
{
	virtual PxGeometryHolder CreateGeometry() override;

public:
	BoxCollider();
	~BoxCollider();

public:

	float GetExtentX();
	float GetExtentY();
	float GetExtentZ();
	void SetExtents(float extentX, float extentY, float extentZ);

private:

	PxBoxGeometry CreateBoxGeometry() const;

private:
	float m_extentX{ 1.f }, m_extentY{ 1.f }, m_extentZ{ 1.f };
};