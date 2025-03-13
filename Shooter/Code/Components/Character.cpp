#include <StdAfx.h>
#include "Character.h"


#include <DefaultComponents/Cameras/CameraComponent.h>
//Start registration stuff
#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/EnumFlags.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryCore/StaticInstanceList.h>


static void RegisterCharacterComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CCharacterComponent));
        {
        }
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCharacterComponent)

//end registration stuff
void CCharacterComponent::Initialize()
{
    m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
    m_pAdvancedAnimationController = m_pEntity->GetOrCreateComponent< Cry::DefaultComponents::CAdvancedAnimationComponent>();
 

}

void CCharacterComponent::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case Cry::Entity::EEvent::GameplayStarted:
    {
        
    }
    break;
    case Cry::Entity::EEvent::Update:
    {
        
    }
    break;
    case Cry::Entity::EEvent::Reset:
    {
        m_MovementDirection = ZERO;
        m_MovementInput = ZERO;
    }
    break;
    default:
        break;
    }
}



Cry::Entity::EventFlags CCharacterComponent::GetEventMask() const
{
    return Cry::Entity::EEvent::Update |
    Cry::Entity::EEvent::Reset |
    Cry::Entity::EEvent::GameplayStarted
    ;
}

void CCharacterComponent::MovementInput(Vec2 NewInput)
{
    
    m_MovementInput = NewInput;
    if (m_MovementInput==ZERO)
    {
        m_MovementDirection = ZERO;
    }
    else
    {
        m_MovementDirection.x = m_MovementInput.x;
        m_MovementDirection.y = m_MovementInput.y;
        m_MovementDirection.z = 0;
        m_MovementDirection = m_pEntity->GetWorldRotation() * m_MovementDirection;
        m_MovementDirection.Normalize();
    }
    Vec3 NewVelocity = m_MovementDirection * m_MovementSpeed;
    m_pCharacterController->SetVelocity(NewVelocity);
    //CryLog("Updated movement Direction: %f,%f,%f", m_MovementDirection.x,m_MovementDirection.y,m_MovementDirection.z);m
}


