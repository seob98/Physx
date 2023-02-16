#include "CapsuleCollider.h"

PxGeometryHolder CapsuleCollider::CreateGeometry()
{
	return PxGeometryHolder();

}

PxCapsuleGeometry CapsuleCollider::CreateCapsuleGeometry() const
{
    PxCapsuleGeometry capsule;
    capsule.radius = m_radius;
    capsule.halfHeight = m_halfHeight;
    return capsule;
}
