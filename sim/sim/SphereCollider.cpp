#include "framework.h"
#include "SphereCollider.h"

PxGeometryHolder SphereCollider::CreateGeometry()
{
    return CreateSphereGeometry();
}

SphereCollider::SphereCollider()
{
}

SphereCollider::~SphereCollider()
{
}

float SphereCollider::GetRadius() const
{
    return m_radius;
}

void SphereCollider::SetRadius(float value)
{
    m_radius = value;
    ResetShape();
}

PxSphereGeometry SphereCollider::CreateSphereGeometry() const
{
    PxSphereGeometry sphere;
                                                        //transform scale ��������
    sphere.radius = m_radius;                           //* GetBiggestLengthFromAbsVec3(transform->scale);
    return sphere;
}

float SphereCollider::GetBiggestLengthFromAbsVec3(float valueX, float valueY, float valueZ) const
{
    float maxNum = abs(valueX);
    maxNum = max(maxNum, abs(valueY));
    maxNum = max(maxNum, abs(valueZ));
    return maxNum;
}
