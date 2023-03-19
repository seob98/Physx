#pragma once

#include "Client_Defines.h"

//	�߿�!
//	shape�� extent ������ �ش� shape�� ����ϴ� ��� ���� ������Ʈ�� ������ �ش�.
//	�� ���� ���(mat�� �ݹ�,���� ��� + shape�� filterData + mat/shape�� flag�� ���ſ� ������ ��������� ������ �� �ش�)

//	restitution(�ݹ߷�)�� mat, shape ���ÿ� �ִ�. ������ shape�� �ݹ߷� ����� ���̸� �δ°����� ����ϳ�
//	mat�� �����ϴ� ������Ʈ�� �ݹ߰���� �Ѳ����� �����ϱ� ���� ����ص� ����.

enum class PhysicsCombineMode;
enum class ColliderShape;
enum class CollisionInfoType;

class CollisionPairInfo;

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


#pragma region oldMatManagementFunctions
	bool CheckIfSameMaterial(PxMaterial* mat1, PxMaterial* mat2);
	void ManageDuplicateMaterials(PxMaterial*& newMat);
#pragma endregion

protected:
	virtual PxGeometryHolder CreateGeometry() = 0;
	void ResetShape();

public:
	// ���� �ùķ��̼� ��Ȳ :
	// shape�� attach�ϱ� ���� ��� �Ұ�. �׷��� ���� Init���� rigidBody�� ���ڷ� ��� �´�.

	// value 0~31
	uint8_t GetLayerIndex() const;
	void SetLayerIndex(uint8_t layerIndex);

	RigidBody* GetRigidBody() const;
	PxShape* GetPxShape() const;

	float GetFriction(float value) const;
	void SetFriction(float value);

	//FrictionFlag
	PhysicsCombineMode GetFrictionCombineMode() const;
	void SetFrictionCombineMode(PhysicsCombineMode value);
	//RestitutionFlag
	PhysicsCombineMode GetRestitutionCombineMode() const;
	void SetRestitutionCombineMode(PhysicsCombineMode value);

public:
	void CollectCollisionInfo(CollisionInfoType type, shared_ptr<CollisionPairInfo> info);
	void ClearCollisionInfo();
	const vector<shared_ptr<CollisionPairInfo>>& GetCollisionInfo(CollisionInfoType type) const;

protected:
	RigidBody* m_OwnerBody = nullptr;

#pragma region oldMatVariables

	int m_materialIndex;
	vector<PxMaterial*> m_materials;

#pragma endregion

	PxMaterial* m_material = nullptr;
	PxShape* m_shape = nullptr;

	bool m_isTrigger = false;
	uint8_t m_layerIndex = 0;
	uint32_t m_ignoreLayerBits = 0x00000000;


	float scaleX = 1.f;
	float scaleY = 1.f;
	float scaleZ = 1.f;

public:
	vector<shared_ptr<CollisionPairInfo>> m_CollisionEnter;
	vector<shared_ptr<CollisionPairInfo>> m_CollisionExit;
	vector<shared_ptr<CollisionPairInfo>> m_CollisionStay;
};

