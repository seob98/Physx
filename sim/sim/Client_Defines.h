#pragma once
#include "Define.h"

#include "InputDevice.h"
#include "PhysDevice.h"

//physX
#include "PhysicsRay.h"
#include "RaycastHit.h"
#include "PhysQuery.h"
#include "Collider.h"
#include "RigidBody.h"
#include "ControllerWrapper.h"
#include "ControllerManagerWrapper.h"
#include "MyFilterShader.h"
#include "CustomQueryFilterCallback.h"		//RaycasteventCallback
#include "MySimulationEventCallback.h"
#include "CollisionPairInfo.h"

#include "Player.h"

//physx collider
enum class PhysicsCombineMode
{
	Average = PxCombineMode::eAVERAGE,
	Min = PxCombineMode::eMIN,
	Multiply = PxCombineMode::eMULTIPLY,
	Max = PxCombineMode::eMAX,
};

enum class ColliderShape
{
	COLLIDER_BOX,
	COLLIDER_SPHERE,
	COLLIDER_CAPSULE,
	COLLIDER_MESH
};

//physx RigidBody
enum class Interpolate
{
	None,
	Interpolate,
	Extrapolate,
};

enum class PhysicsAxis
{
	NONE = 0,			//0000
	X = 1 << 0,			//0001
	Y = 1 << 1,			//0010
	Z = 1 << 2,			//0100
	All = X | Y | Z,	//0111
	//AND, OR연산으로 확인 가능
};

enum class ForceMode
{
	//가속의 추가. 이미 움직이고 있는 물체에 적용하기 적합.
	//PxForceMode::eFORCE					무게 적용
	//PxForceMode::eACCELERATION			무게 무시

	//순간적으로 속도를 적용. 점프에 사용하기 적합
	//PxForceMode::eIMPULSE					무게 적용
	//PxForceMode::eVELOCITY_CHANGE			무게 무시

	Impulse = PxForceMode::eIMPULSE,
	Force = PxForceMode::eFORCE,
	Acceleration = PxForceMode::eACCELERATION,
	Velocity = PxForceMode::eVELOCITY_CHANGE,
};

enum class PhysicsQueryType
{
	None = 0,
	Collider = 1 << 0,
	Trigger = 1 << 1,
	All = Collider | Trigger,
};

enum class PhysicsLayers
{
	Map = 0,
	Player = 1
};

enum class CollisionInfoType
{
	Enter = 0,
	Stay = 1,
	Exit = 2
};