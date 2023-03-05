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
    
}

void ControllerWrapper::Jump()
{
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

void ControllerWrapper::SetSerial(int serial)
{
    m_serial = serial;
}
