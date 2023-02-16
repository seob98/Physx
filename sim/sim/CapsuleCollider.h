#pragma once

#include "Collider.h"

class CapsuleCollider : Collider
{
	virtual PxGeometryHolder CreateGeometry() override;

private:

	PxCapsuleGeometry CreateCapsuleGeometry() const;

private:

	float m_radius = 0.5f;

	float m_halfHeight = 0.5f;
};
};

