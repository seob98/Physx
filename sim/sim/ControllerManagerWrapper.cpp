#include "framework.h"
#include "ControllerManagerWrapper.h"

void ControllerManagerWrapper::Init()
{
	auto phys = PhysDevice::GetInstance();
	m_scene = phys->GetScene();

    m_manager = PxCreateControllerManager(*m_scene);

    m_manager->setOverlapRecoveryModule(true); // Enable overlap recovery module
}

void ControllerManagerWrapper::CreateController()
{
    auto phys = PhysDevice::GetInstance();
    m_scene = phys->GetScene();

    // Create the capsule controller
    PxCapsuleControllerDesc capsuleDesc;
    capsuleDesc.height = 4.5f;
    capsuleDesc.radius = 2.0f;
    capsuleDesc.material = phys->GetDefaultMaterial();
    capsuleDesc.position = PxExtendedVec3(0.f, 10.f, 0.f);
    capsuleDesc.upDirection = PxVec3(0.0f, 1.0f, 0.0f);
    capsuleDesc.slopeLimit = cosf(PxPi / 3.0f);        //60degress
    capsuleDesc.contactOffset = 0.1f;
    capsuleDesc.stepOffset = 0.1f;

    // CreateController + SetFlags
    PxCapsuleController* controller = static_cast<PxCapsuleController*>(m_manager->createController(capsuleDesc));
    PxRigidDynamic* controllerActor = controller->getActor();
    controllerActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
    controllerActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
    controller->setNonWalkableMode(PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING);

    // Lock the rotation of the actor along the axis perpendicular to the up direction
    //controllerActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
    
    // WrappingClass
    ControllerWrapper* wrapper = new ControllerWrapper;
    wrapper->SetController(controller);
    controller->getActor()->userData = static_cast<void*>(wrapper);

    wrapper->SetDensity(1.0f);

    // Place the controller to the vector
    static int serialNum = 0;
    wrapper->SetSerial(serialNum++);
    m_controllerWrappers.emplace_back(wrapper);
}

void ControllerManagerWrapper::UpdateControllers()
{
    //PxVec3 input{};

    //if (InputDevice::GetInstance()->GetKey(Key::Left))
    //    input.x = -1;
    //if (InputDevice::GetInstance()->GetKey(Key::Right))
    //    input.x = 1;
    //if (InputDevice::GetInstance()->GetKey(Key::Up))
    //    input.z = 1;
    //if (InputDevice::GetInstance()->GetKey(Key::Down))
    //    input.z = -1;

    for (auto& wrapper: m_controllerWrappers)
    {
        PxCapsuleController* controller = wrapper->GetController();
        controller->setUpDirection(PxVec3(0.f,1.f,0.f));
    }
}

PxControllerManager* ControllerManagerWrapper::GetControllerManager()
{
    return m_manager;
}
