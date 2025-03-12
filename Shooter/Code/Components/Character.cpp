#include <StdAfx.h>
#include "Character.h"

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
    case Cry::Entity::EEvent::Initialize:
    {

    }
    break;

    case Cry::Entity::EEvent::Update:
    {
        MovementUpdate();
        
    }
    break;
    case Cry::Entity::EEvent::Reset:
    {
        
    }
    break;
    case Cry::Entity::EEvent::GameplayStarted:
    {
        
    }
    break;
    default:
        break;
    }
}

Cry::Entity::EventFlags CCharacterComponent::GetEventMask() const
{
    return Cry::Entity::EEvent::Update |
    Cry::Entity::EEvent::Initialize |
    Cry::Entity::EEvent::Reset |
    Cry::Entity::EEvent::GameplayStarted
    ;
}


void CCharacterComponent::MovementUpdate()
{
    Vec3 MovementDirection = Vec3(m_MovementDirection.x, m_MovementDirection.y, 0);
    MovementDirection.Normalize();
    m_pCharacterController->SetVelocity(m_pEntity->GetWorldRotation()*MovementDirection*m_MovementSpeed);
}
