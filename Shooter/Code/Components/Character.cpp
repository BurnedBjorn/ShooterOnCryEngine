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
    m_pStaticMesh = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CStaticMeshComponent>();
    m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
    m_pAdvancedAnimationController = m_pEntity->GetOrCreateComponent< Cry::DefaultComponents::CAdvancedAnimationComponent>();
    if (m_pAdvancedAnimationController)
    {
        m_movementFragmentID=m_pAdvancedAnimationController->GetFragmentId("Movement");
        m_aimposeFragmentID = m_pAdvancedAnimationController->GetFragmentId("AimPose");
        //Matrix34 NewRot = m_pAdvancedAnimationController->GetTransformMatrix();
        //NewRot.SetRotationXYZ(Ang3(0, 0, 3.14));
        
        //m_pAdvancedAnimationController->SetTransformMatrix(NewRot);
    }

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
        AnimationUpdate();
    }
    break;
    case Cry::Entity::EEvent::Reset:
    {
        ResetAimPose();
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
        
        if (IAttachment* pAttachmentWeapon = m_pAdvancedAnimationController->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("Weapon"))
        {
            
            CryLog("FOUND");
            QuatTS WAbs = pAttachmentWeapon->GetAttWorldAbsolute();
            Matrix34 NewTransform(WAbs);
            
            m_pWeapon->GetEntity()->SetWorldTM(NewTransform);

            CEntityAttachment* pEntityAttachment = new CEntityAttachment;
            pEntityAttachment->SetEntityId(m_pWeapon->GetEntityId());
            
            pAttachmentWeapon->AddBinding(pEntityAttachment);

        }
        else {
            m_pEntity->AttachChild(m_pWeapon->GetEntity());
            Matrix34 WeaponNewTm = m_pWeapon->GetEntity()->GetWorldTM();
            Quat NewRotation = m_pEntity->GetWorldRotation();
            Vec3 NewTranslation = m_pEntity->GetWorldTM().GetTranslation() + gEnv->pSystem->GetViewCamera().GetViewdir() + Vec3(0, 0, 2);
            WeaponNewTm.SetTranslation(NewTranslation);
            WeaponNewTm.SetRotation33(Matrix33(NewRotation));
            m_pWeapon->GetEntity()->SetWorldTM(WeaponNewTm);
        }
        
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
        if (IAttachment* pAttachmentWeapon = m_pAdvancedAnimationController->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("Weapon")) {
            pAttachmentWeapon->ClearBinding();
        }
        m_pWeapon->Drop();
        m_pWeapon = nullptr;

    }
}

void CCharacterComponent::Attack(int activationMode)
{
    if (m_pWeapon)
    {
        m_pWeapon->SetTarget(m_AimTarget);
        if ( activationMode == eAAM_OnPress || activationMode == eAAM_OnRelease||true)
        {
            m_pWeapon->Trigger(activationMode);
        }
    }
    
    
    
    
    
    
    
    //Testing
    /*
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
    */
}

void CCharacterComponent::SetAimTarget(Vec3 NewTarget)
{
    m_AimTarget = NewTarget;
}

void CCharacterComponent::HitDebug(int ipart)
{
    


    BodyPart hitPart = IndexToBodyPart(ipart);
    switch (hitPart)
    {
    case CCharacterComponent::HEAD:
        CryLog("Head");
        break;
    case CCharacterComponent::BODY:
        CryLog("Body");
        break;
    case CCharacterComponent::ARMS:
        CryLog("Arms");
        break;
    case CCharacterComponent::LEGS:
        CryLog("Legs");
        break;
    default:
        CryLog("NotMapped");
        break;
    }
}

void CCharacterComponent::ResetAimPose()
{
    if (m_pAdvancedAnimationController)
    {
        //m_pAdvancedAnimationController->QueueFragmentWithId(m_aimposeFragmentID);
        if (m_pAdvancedAnimationController->GetCharacter())
        {
            if (ISkeletonPose* pSkeletonPose = m_pAdvancedAnimationController->GetCharacter()->GetISkeletonPose())
            {
                if (IAnimationPoseBlenderDir* pAimPoseBlenderDir = pSkeletonPose->GetIPoseBlenderAim()) {

                    pAimPoseBlenderDir->SetState(true);
                    pAimPoseBlenderDir->SetLayer(0);
                    pAimPoseBlenderDir->SetTarget(m_AimTarget);
                    pAimPoseBlenderDir->SetFadeInSpeed(0.001);
                    pAimPoseBlenderDir->SetPolarCoordinatesSmoothTimeSeconds(0.05);
                    pAimPoseBlenderDir->SetFadeOutSpeed(0.001);


                };
            }
        }


    }
}

void CCharacterComponent::AnimationUpdate()
{
    if (m_pAdvancedAnimationController)
    {
        //m_pAdvancedAnimationController->QueueFragmentWithId(m_aimposeFragmentID);
        if (m_pAdvancedAnimationController->GetCharacter())
        {
            if (ISkeletonPose* pSkeletonPose = m_pAdvancedAnimationController->GetCharacter()->GetISkeletonPose())
            {
                if (IAnimationPoseBlenderDir* pAimPoseBlenderDir = pSkeletonPose->GetIPoseBlenderAim()) {

                    
                    pAimPoseBlenderDir->SetTarget(m_AimTarget);
                    
                    
                    
                };
            }
        }
        
        
    }
}




