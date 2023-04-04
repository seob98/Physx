#pragma once

#include "Client_Defines.h"

class Collider;

class TriangleMeshCollider : public Collider
{
	virtual PxGeometryHolder CreateGeometry() override;

public:
	TriangleMeshCollider();
	~TriangleMeshCollider();
public:
	void Init(RigidBody* body);

protected:
	PxConvexMeshGeometry CreateTriangleMeshGeometry();
private:
	PxConvexMesh* m_mesh;
};

