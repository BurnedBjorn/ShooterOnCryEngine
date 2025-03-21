#include <StdAfx.h>
#include "PlayerController.h"


#include <CryRenderer/IRenderAuxGeom.h>


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


static void RegisterPlayerControllerComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerController));
        {
        }
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerControllerComponent)

void CPlayerController::Initialize()
{
    m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
    m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
    m_pControlledCharacter = m_pEntity->GetOrCreateComponent<CCharacterComponent>();

    
}

void CPlayerController::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case Cry::Entity::EEvent::Update:
    {
        /*
        ray_hit ViewRay;
        Vec3 CamDir = gEnv->pSystem->GetViewCamera().GetViewdir();
        Vec3 CamLoc = gEnv->pSystem->GetViewCamera().GetPosition();
        static const unsigned int RayFlags = rwi_stop_at_pierceable | rwi_colltype_any;
        if (gEnv->pPhysicalWorld->RayWorldIntersection(CamLoc, (CamDir*1000),ent_all, RayFlags, &ViewRay, 1,m_pEntity->GetPhysicalEntity()))
        {
            gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(ViewRay.pt - Vec3(0, 0, 0.5), ColorB(0, 0, 255), ViewRay.pt + Vec3(0, 0, 0.5), ColorB(0, 0, 255), 6.0f);
            gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(ViewRay.pt - Vec3(0, 0.5, 0), ColorB(0, 0, 255), ViewRay.pt + Vec3(0, 0.5, 0), ColorB(0, 0, 255), 6.0f);
            gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(ViewRay.pt - Vec3(0.5, 0, 0), ColorB(0, 0, 255), ViewRay.pt + Vec3(0.5, 0, 0), ColorB(0, 0, 255), 6.0f);
        }
        */
    }
    break;
    case Cry::Entity::EEvent::GameplayStarted:
    {
        
        if (m_pControlledCharacter && m_pInputComponent)
        {
            InitializeInput();
        }
        m_EntityOrientation = m_pEntity->GetRotation();
        
    }
    break;
    case Cry::Entity::EEvent::Reset:
    {
        m_MovementInput = ZERO;
        m_LookInput = ZERO;
        m_EntityOrientation = IDENTITY;

        Vec3 CameraDefaultLocation = ZERO;
        CameraDefaultLocation.z = m_CameraDefaultHeight;
        m_CameraTransform.SetTranslation(CameraDefaultLocation);
        m_CameraTransform.SetRotation33(Matrix33(m_pEntity->GetWorldRotation()));
        m_pCameraComponent->SetTransformMatrix(m_CameraTransform);
    }
    break;
    default:
        break;
    }
}

Cry::Entity::EventFlags CPlayerController::GetEventMask() const
{
    return Cry::Entity::EEvent::Update|
        Cry::Entity::EEvent::GameplayStarted|
        Cry::Entity::EEvent::Reset;
}

void CPlayerController::InitializeInput()
{
    
    m_pInputComponent->RegisterAction("PlayerMovement", "Forward", [this](int activationMode, float value) {m_MovementInput.y = value; SendMovementUpdate(); });
    m_pInputComponent->BindAction("PlayerMovement", "Forward", eAID_KeyboardMouse, eKI_W);

    m_pInputComponent->RegisterAction("PlayerMovement", "Back", [this](int activationMode, float value) { m_MovementInput.y = -value; SendMovementUpdate(); });
    m_pInputComponent->BindAction("PlayerMovement", "Back", eAID_KeyboardMouse, eKI_S);

    m_pInputComponent->RegisterAction("PlayerMovement", "Right", [this](int activationMode, float value) {m_MovementInput.x = value; SendMovementUpdate(); });
    m_pInputComponent->BindAction("PlayerMovement", "Right", eAID_KeyboardMouse, eKI_D);

    m_pInputComponent->RegisterAction("PlayerMovement", "Left", [this](int activationMode, float value) {m_MovementInput.x = -value; SendMovementUpdate(); });
    m_pInputComponent->BindAction("PlayerMovement", "Left", eAID_KeyboardMouse, eKI_A);
    
    m_pInputComponent->RegisterAction("PlayerLooking", "Vertical", [this](int activationMode, float value) {m_LookInput.y = -value; CameraRotationUpdate();  });
    m_pInputComponent->BindAction("PlayerLooking", "Vertical", eAID_KeyboardMouse, eKI_MouseY);

    m_pInputComponent->RegisterAction("PlayerLooking", "Horisontal", [this](int activationMode, float value) {m_LookInput.x = -value; EntityRotationUpdate(); });
    m_pInputComponent->BindAction("PlayerLooking", "Horisontal", eAID_KeyboardMouse, eKI_MouseX);

    m_pInputComponent->RegisterAction("PlayerAction", "Use", [this](int activationMode, float value) {Use(); });
    m_pInputComponent->BindAction("PlayerAction", "Use", eAID_KeyboardMouse, eKI_E);

    m_pInputComponent->RegisterAction("PlayerAction", "Drop", [this](int activationMode, float value) {Drop(); });
    m_pInputComponent->BindAction("PlayerAction", "Drop", eAID_KeyboardMouse, eKI_G);
    

    m_pInputComponent->RegisterAction("PlayerAction", "Attack", [this](int activationMode, float value) {Attack(activationMode); });
    m_pInputComponent->BindAction("PlayerAction", "Attack", eAID_KeyboardMouse, eKI_Mouse1);
    
}

void CPlayerController::Use()
{
    ray_hit Ray;
    Vec3 CamDir = gEnv->pSystem->GetViewCamera().GetViewdir();
    Vec3 CamLoc = gEnv->pSystem->GetViewCamera().GetPosition();
    static const unsigned int RayFlags = rwi_stop_at_pierceable | rwi_colltype_any;
    if (gEnv->pPhysicalWorld->RayWorldIntersection(CamLoc, (CamDir * 5), ent_all, RayFlags, &Ray, 1, m_pEntity->GetPhysicalEntity())) {
        if (Ray.pCollider)
        {
            if (IEntity* pColliderEntity=gEnv->pEntitySystem->GetEntityFromPhysics(Ray.pCollider))
            {
                if (CWeaponComponent* pWeaponComponent = pColliderEntity->GetComponent<CWeaponComponent>())
                {
                    gEnv->pRenderer->GetIRenderAuxGeom()->RenderText(Ray.pt, SDrawTextInfo(),"Test",va_list());
                    PickUp(pWeaponComponent);
                }
            }
        }
        gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(Ray.pt - Vec3(0, 0, 0.5), ColorB(0, 0, 255), Ray.pt + Vec3(0, 0, 0.5), ColorB(0, 0, 255), 6.0f);
        gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(Ray.pt - Vec3(0, 0.5, 0), ColorB(0, 0, 255), Ray.pt + Vec3(0, 0.5, 0), ColorB(0, 0, 255), 6.0f);
        gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(Ray.pt - Vec3(0.5, 0, 0), ColorB(0, 0, 255), Ray.pt + Vec3(0.5, 0, 0), ColorB(0, 0, 255), 6.0f);
    }
}

void CPlayerController::PickUp(CWeaponComponent* pWeapon)
{
    m_pControlledCharacter->PickUpWeapon(pWeapon);
}

void CPlayerController::Drop()
{
    m_pControlledCharacter->DropWeapon();
}

void CPlayerController::Attack(int activationMode)
{
    ray_hit target;
    Vec3 CamDir = gEnv->pSystem->GetViewCamera().GetViewdir();
    Vec3 CamLoc = gEnv->pSystem->GetViewCamera().GetPosition();
    Vec3 AttackTarget;
    static const unsigned int RayFlags = rwi_stop_at_pierceable | rwi_colltype_any;
    if (gEnv->pPhysicalWorld->RayWorldIntersection(CamLoc, (CamDir * 1000), ent_all, RayFlags, &target, 1, m_pEntity->GetPhysicalEntity()))
    {
        AttackTarget = target.pt;
    }
    else
    {
        AttackTarget = CamLoc + CamDir * 1000;
    }
    
}

void CPlayerController::SendMovementUpdate()
{
    m_pControlledCharacter->MovementInput(m_MovementInput);
    EActionActivationMode::
}

void CPlayerController::EntityRotationUpdate()
{
    
    Ang3 NewEntityAngles = CCamera::CreateAnglesYPR(Matrix33(m_EntityOrientation));
    NewEntityAngles.x += m_LookInput.x * m_MouseSensitivity;
    NewEntityAngles.z = 0;
    m_EntityOrientation = Quat(CCamera::CreateOrientationYPR(NewEntityAngles));
    m_pEntity->SetRotation(m_EntityOrientation);
}

void CPlayerController::CameraRotationUpdate()
{
    Ang3 NewCameraAngles = CCamera::CreateAnglesYPR(Matrix33(Quat(m_CameraTransform)));
    NewCameraAngles.y = CLAMP(NewCameraAngles.y+m_LookInput.y * m_MouseSensitivity,m_CamYRotMin,m_CamYRotMax);
    m_CameraTransform.SetRotation33(Matrix33(CCamera::CreateOrientationYPR(NewCameraAngles)));
    m_pCameraComponent->SetTransformMatrix(m_CameraTransform);
}


