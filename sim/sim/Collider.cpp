#include "Collider.h"

void Collider::Init()
{
	auto phys = PhysDevice::GetInstance()->GetPhysics();

	m_material = phys->createMaterial(0.5f, 0.5f, 0.1f);
	m_material->setFrictionCombineMode(PxCombineMode::eMIN);

	m_shape = phys->createShape(CreateGeometry().any(), *m_material, true);
	m_shape->userData = this;
}
