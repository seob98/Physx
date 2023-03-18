#pragma once
#include "Client_Defines.h"

class Collider;

class CapsuleCollider : public Collider
{
	virtual PxGeometryHolder CreateGeometry() override;

public:
	CapsuleCollider();
	~CapsuleCollider();

public:

	float GetRadius();
	float GetHalfHeight();
	void SetRadius(float value);
	void SetHalfHeight(float value);

private:
	PxCapsuleGeometry CreateCapsuleGeometry(float radius, float halfHeight);

private:
	float m_radius = 0.5f;
	float m_halfHeight = 0.5f;
};
