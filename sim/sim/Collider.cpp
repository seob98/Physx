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
	
#pragma region oldMatManagement
	//PxMaterial* newMat = phys->createMaterial(1.f, 1.f, 0.f);
	//newMat->setFrictionCombineMode(PxCombineMode::eMIN);
	//newMat->setRestitutionCombineMode(PxCombineMode::eMIN);			//apply material flags
	//ManageDuplicateMaterials(newMat);								//check for duplicates														
#pragma endregion

	m_material = phys->createMaterial(1.f, 1.f, 0.f);

	//shape
	m_shape = phys->createShape(CreateGeometry().any(), *m_material, true);
	m_shape->userData = this;

	m_OwnerBody = body;
	m_OwnerBody->Attach(this);

	ApplyShapeFlags();
	ApplyTransform();
	ApplyLayer();
}

void Collider::ApplyShapeFlags()
{
	// Ʈ���� �÷���: Ʈ���� ���
	// �ùķ��̼� �÷���: Ʈ���� �÷��װ� ���� ������ Ȱ��ȭ
	// ���� �÷���: ������Ʈ�� Ȱ��ȭ ���¶�� ������ ���� �ִ�.

	bool triggerFlag = false;
	bool simulationFlag = !triggerFlag;
	bool queryFlag = true;

	// eTRIGGER_SHAPE �÷��׸� Ȱ��ȭ �ϱ� ������ �ݵ�� eSIMULATION_SHAPE �÷��װ� ��Ȱ��ȭ ���¿��� �մϴ�.
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
	auto device = PhysDevice::GetInstance();
	
	// ������ ���̾ ������ ��� ��Ʈ�� �մϴ�.
	uint32_t unignoreBits = ~m_ignoreLayerBits;

	PxFilterData filter{};

	// ���̾� ��Ʈ�� �մϴ�.
	filter.word0 = (1 << m_layerIndex);

	// �������� �ʴ� ��Ʈ���� �մϴ�.
	filter.word1 = unignoreBits;

	m_shape->setSimulationFilterData(filter);
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

uint8_t Collider::GetLayerIndex() const
{
	return m_layerIndex;
}

void Collider::SetLayerIndex(uint8_t layerIndex)
{
	assert(layerIndex < 32);

	m_layerIndex = layerIndex;

	ApplyLayer();
}

RigidBody* Collider::GetRigidBody() const
{
	PxRigidActor* actor = m_shape->getActor();
	if (!actor)
		return nullptr;

	return (RigidBody*)actor->userData;
}

PxShape* Collider::GetPxShape() const
{
	return m_shape;
}

float Collider::GetFriction(float value) const
{
	return m_material->getDynamicFriction();
}

void Collider::SetFriction(float value)
{
	m_material->setStaticFriction(value);
	m_material->setDynamicFriction(value);
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

void Collider::CollectCollisionInfo(CollisionInfoType type, shared_ptr<CollisionPairInfo> info)
{
	switch (type)
	{
	case CollisionInfoType::Enter:
		m_CollisionEnter.emplace_back(info);
		break;
	case CollisionInfoType::Stay:
		m_CollisionStay.emplace_back(info);
		break;
	case CollisionInfoType::Exit:
		m_CollisionExit.emplace_back(info);
		break;
	}
}

void Collider::ClearCollisionInfo()
{
	m_CollisionEnter.clear();
	m_CollisionStay.clear();
	m_CollisionExit.clear();
}

const vector<shared_ptr<CollisionPairInfo>>& Collider::GetCollisionInfo(CollisionInfoType type) const
{
	switch (type)
	{
	case CollisionInfoType::Enter:
		return m_CollisionEnter;
	case CollisionInfoType::Stay:
		return m_CollisionStay;
	case CollisionInfoType::Exit:
		return m_CollisionExit;
	}
}



