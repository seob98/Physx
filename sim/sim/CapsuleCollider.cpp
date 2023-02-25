//#include "CapsuleCollider.h"
//
//PxGeometryHolder CapsuleCollider::CreateGeometry()
//{
//	return CreateCapsuleGeometry(scaleX, scaleY, scaleZ);
//}
//
//float CapsuleCollider::GetRadius() const
//{
//    return m_radius;
//}
//
//void CapsuleCollider::SetRadius(float value)
//{
//    m_radius = value;
//    ResetShape();
//}
//
//float CapsuleCollider::GetHalfHeight() const
//{
//    return m_halfHeight;
//}
//
//void CapsuleCollider::SetHalfHeight(float value)
//{
//    m_halfHeight = value;
//    ResetShape();
//}
//
//PxCapsuleGeometry CapsuleCollider::CreateCapsuleGeometry(float scaleX, float scaleY, float scaleZ) const
//{
//    if (scaleY < 0)
//        scaleY *= -1;
//
//    PxCapsuleGeometry capsule;
//    capsule.radius = m_radius * GetBiggestLengthFromAbsVec3XZ(scaleX, scaleY, scaleZ);
//    capsule.halfHeight = m_halfHeight * scaleY;
//    return capsule;
//}
//
//float CapsuleCollider::GetBiggestLengthFromAbsVec3XZ(float scaleX, float scaleY, float scaleZ) const
//{
//    if (scaleX < 0)
//        scaleX *= -1;
//    if (scaleY < 0)
//        scaleY *= -1;
//    if (scaleZ < 0)
//        scaleZ *= -1;
//
//    float max = max(scaleX, scaleZ);
//    return max;
//}
