#include "framework.h"
#include "Collider.h"


Collider::Collider()
{
}

Collider::~Collider()
{
}

void Collider::Init(RigidBody* body)
{
	auto phys = PhysDevice::GetInstance()->GetPhysics();
	//mat
	PxMaterial* newMat = phys->createMaterial(0.5f, 0.5f, 0.6f);
	newMat->setFrictionCombineMode((PxCombineMode::Enum)PhysicsCombineMode::Min);
	newMat->setRestitutionCombineMode((PxCombineMode::Enum)PhysicsCombineMode::Min);		//apply material flags
	ManageDuplicateMaterials(newMat);														//check for duplicates														
	
	//shape
	m_shape = phys->createShape(CreateGeometry().any(), *newMat, true);
	m_shape->userData = this;

	m_OwnerBody = body;
	m_OwnerBody->Attach(this);

	ApplyShapeFlags();
	ApplyTransform();
	ApplyLayer();
}

void Collider::ApplyShapeFlags()
{
	// 트리거 플래그: 트리거 모드
	// 시뮬레이션 플래그: 트리거 플래그가 꺼져 있으면 활성화
	// 쿼리 플래그: 컴포넌트가 활성화 상태라면 언제나 켜져 있다.

	bool triggerFlag = false;
	bool simulationFlag = !triggerFlag;
	bool queryFlag = true;

	// eTRIGGER_SHAPE 플래그를 활성화 하기 전에는 반드시 eSIMULATION_SHAPE 플래그가 비활성화 상태여야 합니다.
	m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, simulationFlag);
	m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, triggerFlag);
	m_shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, queryFlag);
}

void Collider::ApplyTransform()
{
	PxTransform t = GetRigidBody()->GetBody()->getGlobalPose();
	PxTransform t2(PxIdentity);
	m_shape->setLocalPose(t);
}

void Collider::ApplyScale()
{
	ResetShape();
}

void Collider::ApplyLayer()
{
	//auto device = PhysDevice::GetInstance();
	////layerMgr 이후 사용
	//
	//// 무시할 레이어를 제외한 모든 비트를 켭니다.
	//uint32_t unignoreBits = ~m_ignoreLayerBits;

	//PxFilterData filter{};

	//// 레이어 비트를 켭니다.
	//filter.word0 = (1 << m_layerIndex);

	//// 무시하지 않는 비트들을 켭니다.
	//filter.word1 = unignoreBits;

	//m_shape->setSimulationFilterData(filter);
}

bool Collider::CheckIfSameMaterial(PxMaterial* mat1, PxMaterial* mat2)
{
	if (mat1->getDynamicFriction() != mat2->getDynamicFriction()) return false;
	if (mat1->getStaticFriction() != mat2->getStaticFriction()) return false;
	if (mat1->getRestitution() != mat2->getRestitution()) return false;
	if (mat1->getFrictionCombineMode() != mat2->getFrictionCombineMode()) return false;
	if (mat1->getRestitutionCombineMode() != mat2->getRestitutionCombineMode()) return false;

	return true;
}

void Collider::ManageDuplicateMaterials(PxMaterial*& newMat)
{
	bool found = false;
	for (size_t i = 0; i < m_materials.size(); i++)
	{
		if (CheckIfSameMaterial(m_materials[i], newMat))
		{
			newMat->release();
			newMat = m_materials[i];
			m_materialIndex = i;
			found = true;
			break;
		}
	}
	if (!found)
	{
		m_materialIndex = m_materials.size();
		m_materials.push_back(newMat);
	}
}

void Collider::ResetShape()
{
	m_shape->setGeometry(CreateGeometry().any());

	RigidBody* body = GetRigidBody();
	if (body)
	{
		body->UpdateMassAndInertia();
	}
}

RigidBody* Collider::GetRigidBody() const
{
	PxRigidActor* actor = m_shape->getActor();
	if (!actor)
		return nullptr;

	return (RigidBody*)actor->userData;
}

PhysicsCombineMode Collider::GetFrictionCombineMode() const
{
	return (PhysicsCombineMode)m_materials[m_materialIndex]->getFrictionCombineMode();
}

void Collider::SetFrictionCombineMode(PhysicsCombineMode value)
{
	m_materials[m_materialIndex]->setFrictionCombineMode((PxCombineMode::Enum)value);
}

PhysicsCombineMode Collider::GetRestitutionCombineMode() const
{
	return (PhysicsCombineMode)m_materials[m_materialIndex]->getRestitutionCombineMode();
}

void Collider::SetRestitutionCombineMode(PhysicsCombineMode value)
{
	m_materials[m_materialIndex]->setRestitutionCombineMode((PxCombineMode::Enum)value);
}
