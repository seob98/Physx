#include "framework.h"
#include "ControllerWrapper.h"

ControllerWrapper::ControllerWrapper()
{
}

ControllerWrapper::~ControllerWrapper()
{
}

void ControllerWrapper::Init()
{
    m_body = m_controller->getActor();
}

void ControllerWrapper::Move()
{
    PxVec3 inputMovements(0.f, 0.f, 0.f);
    float inputValue = 1.f;

    PxVec3 gravity(0.f, -9.81f * 0.5f, 0.f);

    float timeStep = 1.0f / PX_SIM_FRAMECNT;

    if (InputDevice::GetInstance()->GetKey(Key::A))
    {
        inputMovements.x -= inputValue;
    }
    else if (InputDevice::GetInstance()->GetKey(Key::D))
    {
        inputMovements.x += inputValue;
    }
    else if (InputDevice::GetInstance()->GetKey(Key::S))
    {
        inputMovements.z -= inputValue;
    }
    else if (InputDevice::GetInstance()->GetKey(Key::W))
    {
        inputMovements.z += inputValue;
    }

    //combine movements
    physx::PxVec3 displacement = (inputMovements + gravity) * timeStep;

    PxControllerCollisionFlags collisionFlags = m_controller->move(displacement, 0.001f, timeStep, PxControllerFilters(), nullptr);
}

void ControllerWrapper::MoveWithNaturalGrav()
{
    PxVec3 inputMovements(0.f, 0.f, 0.f);
    float inputValue = 1.f;
    float timeStep = 1.f / PX_SIM_FRAMECNT;

    if (InputDevice::GetInstance()->GetKey(Key::A))
    {
        inputMovements.x -= inputValue;
    }
    else if (InputDevice::GetInstance()->GetKey(Key::D))
    {
        inputMovements.x += inputValue;
    }
    else if (InputDevice::GetInstance()->GetKey(Key::S))
    {
        inputMovements.z -= inputValue;
    }
    else if (InputDevice::GetInstance()->GetKey(Key::W))
    {
        inputMovements.z += inputValue;
    }

    // Check if the controller is on the ground
    PxControllerState state;
    m_controller->getState(state);
    bool isOnGround = (state.collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN) ||
        (state.collisionFlags & PxControllerCollisionFlag::eCOLLISION_SIDES);

#pragma region raycastDown
    PxRaycastBuffer hitBuffer;
    PxQueryFilterData filterData;
    PxExtendedVec3 currentFootPos = m_controller->getFootPosition();
    //default contact offset of rigidBody
    PxVec3 origin = PxVec3(currentFootPos.x, currentFootPos.y + 0.2f, currentFootPos.z);
    PxVec3 direction(0.0f, -1.0f, 0.0f);
    PxReal distance = 100.0f;
    auto query = PhysDevice::GetInstance()->GetQuery();
    bool hitGround = query->Raycast(origin, direction, distance, filterData, &hitBuffer);

    // If the raycast hit the ground, set the foot position
    if (hitGround)
    {
        PxReal hitDistance = hitBuffer.block.distance;
        cout << "raycast °Å¸® : " << hitDistance << endl;
    }
#pragma endregion

    // calculate gravity magnitude based on elapsed time
    static float gravityMagnitude = 0.f;
    if (isOnGround)
    {
        gravityMagnitude = 0.f;
    }
    else
    {
        gravityMagnitude += timeStep * 9.81f;
        gravityMagnitude = min(gravityMagnitude, 9.81f);
    }

    PxVec3 gravity(0.f, -gravityMagnitude, 0.f);

    // movements + grav
    physx::PxVec3 displacement = (inputMovements + gravity) * timeStep;

    PxControllerCollisionFlags collisionFlags = m_controller->move(displacement, 0.001f, timeStep, PxControllerFilters(), nullptr);

	//if (isOnGround)
	//	cout << "on ground" << endl;
	//else
	//	cout << "on air" << endl;



}

void ControllerWrapper::Jump()
{

}

void ControllerWrapper::Update()
{
    //Move();
    MoveWithNaturalGrav();
}

PxCapsuleController* ControllerWrapper::GetController()
{
    return m_controller;
}

void ControllerWrapper::SetController(PxCapsuleController* controller)
{
    m_controller = controller;
}

int ControllerWrapper::GetSerial()
{
    return m_serial;
}

void ControllerWrapper::SetDensity(float density)
{
    m_density = density;
    if (m_controller)
    {
        auto actor = m_controller->getActor();
        if (actor)
        {
            actor->setMass(density > 0.0f ? 1.0f / density : PX_MAX_F32);
            actor->setMassSpaceInertiaTensor(PxVec3(1.0f, 1.0f, 1.0f) * actor->getMass());
        }
    }
}

float ControllerWrapper::GetDensity()
{
    return m_density;
}

void ControllerWrapper::SetRotationLockAxis(PhysicsAxis axes, bool value)
{
    PxU32 flag = (PxU32)axes << 3;
    
    PxRigidDynamic* body = m_controller->getActor();

    body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum(flag), value);
}

bool ControllerWrapper::isKinematic() const
{
    PxRigidDynamic* body = m_controller->getActor();
    return body->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC);
}

void ControllerWrapper::SetSerial(int serial)
{
    m_serial = serial;
}
