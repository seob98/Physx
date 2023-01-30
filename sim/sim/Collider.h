#pragma once
class Collider
{
	friend class RigidBody;

protected:
	PxMaterial* m_material = nullptr;
	PxShape* m_shape = nullptr;
};

