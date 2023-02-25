#pragma once

#include "Client_Defines.h"

//	�߿�!
//	shape�� extent ������ �ش� shape�� ����ϴ� ��� ���� ������Ʈ�� ������ �ش�.
//	�� ���� ���(mat�� �ݹ�,���� ��� + shape�� filterData + mat/shape�� flag�� ���ſ� ������ ��������� ������ �� �ش�)

//	restitution(�ݹ߷�)�� mat, shape ���ÿ� �ִ�. ������ shape�� �ݹ߷� ����� ���̸� �δ°����� ����ϳ�
//	mat�� �����ϴ� ������Ʈ�� �ݹ߰���� �Ѳ����� �����ϱ� ���� ����ص� ����.

enum class PhysicsCombineMode;
enum class ColliderShape;

class Collider
{
	friend class RigidBody;

public:
	Collider();
	~Collider();

public:
	void Init(RigidBody* body);
	void ApplyShapeFlags();
	void ApplyTransform();
	void ApplyScale();
	void ApplyLayer();

	bool CheckIfSameMaterial(PxMaterial* mat1, PxMaterial* mat2);
	void ManageDuplicateMaterials(PxMaterial*& newMat);

protected:
	virtual PxGeometryHolder CreateGeometry() = 0;
	void ResetShape();

public:
	// ���� �ùķ��̼� ��Ȳ :
	// shape�� attach�ϱ� ���� ��� �Ұ�. �׷��� ���� Init���� rigidBody�� ���ڷ� ��� �´�. 
	RigidBody* GetRigidBody() const;

	// ���� ���� ��带 ��ȯ
	PhysicsCombineMode GetFrictionCombineMode() const;
	// ���� ���� ��带 ����
	void SetFrictionCombineMode(PhysicsCombineMode value);
	// �ݹ߰�� ���� ��带 ��ȯ
	PhysicsCombineMode GetRestitutionCombineMode() const;
	// �ݹ߰�� ���� ��带 ����
	void SetRestitutionCombineMode(PhysicsCombineMode value);


protected:
	RigidBody* m_OwnerBody = nullptr;

	int m_materialIndex;
	vector<PxMaterial*> m_materials;
	PxShape* m_shape = nullptr;


	bool m_isTrigger = false;
	uint8_t m_layerIndex = 0;
	uint32_t m_ignoreLayerBits = 0x00000000;


	float scaleX = 1.f;
	float scaleY = 1.f;
	float scaleZ = 1.f;
};

