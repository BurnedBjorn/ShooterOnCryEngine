#include <StdAfx.h>
#include "Character.h"


#include <DefaultComponents/Cameras/CameraComponent.h>
//Start registration stuff

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
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
        DropWeapon();
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
    
}

void CCharacterComponent::PickUpWeapon(CWeaponComponent* NewWeapon)
{
    if (!m_pWeapon&&(NewWeapon!=nullptr)&&(!NewWeapon->HasOwner()))
    {
        m_pWeapon = NewWeapon;
        m_pWeapon->PickUp(this);
        m_pEntity->AttachChild(m_pWeapon->GetEntity());
        
        Matrix34 WeaponNewTm = m_pWeapon->GetEntity()->GetWorldTM();
        Quat NewRotation = m_pEntity->GetWorldRotation();
        Vec3 NewTranslation = m_pEntity->GetWorldTM().GetTranslation()+gEnv->pSystem->GetViewCamera().GetViewdir() + Vec3(0,0,2);
        WeaponNewTm.SetTranslation(NewTranslation);
        WeaponNewTm.SetRotation33(Matrix33(NewRotation));
        m_pWeapon->GetEntity()->SetWorldTM(WeaponNewTm);
    }
    else
    {
        if (m_pWeapon)
        {
            CryLog("WeaponAlreadyEquipped");
        }
    }
}

void CCharacterComponent::DropWeapon()
{
    if (m_pWeapon)
    {
        m_pWeapon->Drop();
        m_pWeapon = nullptr;

    }
}

void CCharacterComponent::Attack(Vec3 Target, int activationMode)
{
    if (m_pWeapon)
    {
        m_pWeapon->SetTarget(Target);
        if ( activationMode == eAAM_OnPress || activationMode == eAAM_OnRelease)
        {
            m_pWeapon->Trigger(activationMode);
        }
    }
    
    
    
    
    
    
    //Testing
    if (activationMode == EActionActivationMode::eAAM_OnPress)
    {
        if (m_pWeapon)
        {
            
                //CryLog("Press");
                m_pWeapon->TriggerTest();
            

        }
        else
        {
            CryLog("NO WEAPON");
        }
    }
    
}


