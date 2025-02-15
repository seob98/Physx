#pragma once

#include "Client_Defines.h"

//	중요!
//	shape의 extent 변경은 해당 shape를 사용하는 모든 게임 오브젝트에 영향을 준다.
//	그 외의 요소(mat의 반발,마찰 계수 + shape의 filterData + mat/shape의 flag는 과거에 생성한 결과물에는 영향을 안 준다)

//	restitution(반발력)은 mat, shape 동시에 있다. 보통은 shape의 반발력 계수에 차이를 두는것으로 충분하나
//	mat을 공유하는 오브젝트의 반발계수를 한꺼번에 조정하기 위해 사용해도 좋다.

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
	// 현재 시뮬레이션 상황 :
	// shape를 attach하기 전엔 사용 불가. 그래서 현재 Init에는 rigidBody를 인자로 들고 온다.

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
	void SetMaterialFlag(PxMaterialFlag::Enum, bool value);

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

