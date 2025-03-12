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
    case Cry::Entity::EEvent::Initialize:
    {

    }
    break;
    
    case Cry::Entity::EEvent::Update:
    {
        MovementUpdate();
        
        Ang3 NewRotation = CCamera::CreateAnglesYPR(Matrix33(m_LookOrientation));
        
        NewRotation.x += m_LookInput.x;
        NewRotation.y += m_LookInput.y;
        NewRotation.z = 0;
        
        m_LookOrientation = Quat(CCamera::CreateOrientationYPR(NewRotation));
     
        
        m_pEntity->SetRotation(m_LookOrientation);
        
        
    }
    break;
    case Cry::Entity::EEvent::Reset:
    {
        CryLog("Reset");
        m_LookOrientation = IDENTITY;
    }
    break;
    case Cry::Entity::EEvent::GameplayStarted:
    {
        
        //m_LookOrientation = IDENTITY;
       
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
    if (MovementDirection.GetLength()>0)
    {
        MovementDirection.Normalize();
    }
    else
    {
        MovementDirection = Vec3(0, 0, 0);
    }
    Vec3 Velocity = MovementDirection * m_MovementSpeed;
    if (m_pEntity->GetWorldRotation().IsValid())
    {
        Velocity = m_pEntity->GetWorldRotation()*Velocity;
    }
    else
    {
        Velocity = m_LookOrientation* Velocity;
    }
    m_pCharacterController->SetVelocity(Velocity);
}
