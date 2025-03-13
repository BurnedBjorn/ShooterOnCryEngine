#include <StdAfx.h>
#include "PlayerController.h"
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
        
    }
    break;
    case Cry::Entity::EEvent::GameplayStarted:
    {
        m_MovementInput = ZERO;
        m_LookInput = ZERO;
        if (m_pControlledCharacter && m_pInputComponent)
        {
            InitializeInput();
        }
        
    }
    break;
    case Cry::Entity::EEvent::Reset:
    {
        m_MovementInput = ZERO;
        m_LookInput = ZERO;

        Matrix34 CameraDefaultTransform;
        Vec3 CameraDefaultLocation = ZERO;
        CameraDefaultLocation.z = m_CameraDefaultHeight;
        CameraDefaultTransform.SetTranslation(CameraDefaultLocation);
        CameraDefaultTransform.SetRotation33(Matrix33(m_pEntity->GetWorldRotation()));
        m_pCameraComponent->SetTransformMatrix(CameraDefaultTransform);
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
    
    m_pInputComponent->RegisterAction("PlayerMovement", "Forward", [this](int activationMode, float value) {m_pControlledCharacter->m_MovementDirection.y = value;CryLog("Input"); });
    m_pInputComponent->BindAction("PlayerMovement", "Forward", eAID_KeyboardMouse, eKI_W);

    m_pInputComponent->RegisterAction("PlayerMovement", "Back", [this](int activationMode, float value) { m_pControlledCharacter->m_MovementDirection.y = -value; });
    m_pInputComponent->BindAction("PlayerMovement", "Back", eAID_KeyboardMouse, eKI_S);

    m_pInputComponent->RegisterAction("PlayerMovement", "Right", [this](int activationMode, float value) {m_pControlledCharacter->m_MovementDirection.x = value; });
    m_pInputComponent->BindAction("PlayerMovement", "Right", eAID_KeyboardMouse, eKI_D);

    m_pInputComponent->RegisterAction("PlayerMovement", "Left", [this](int activationMode, float value) {m_pControlledCharacter->m_MovementDirection.x = -value; });
    m_pInputComponent->BindAction("PlayerMovement", "Left", eAID_KeyboardMouse, eKI_A);

    m_pInputComponent->RegisterAction("PlayerLooking", "Vertical", [this](int activationMode, float value) {m_pControlledCharacter->m_LookInput.y = -value * m_MouseSensitivity;  });
    m_pInputComponent->BindAction("PlayerLooking", "Vertical", eAID_KeyboardMouse, eKI_MouseY);

    m_pInputComponent->RegisterAction("PlayerLooking", "Horisontal", [this](int activationMode, float value) {m_pControlledCharacter->m_LookInput.x = -value* m_MouseSensitivity; });
    m_pInputComponent->BindAction("PlayerLooking", "Horisontal", eAID_KeyboardMouse, eKI_MouseX);

}

