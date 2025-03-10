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
#include "Player.h"


static void RegisterPlayerComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerComponent));
        {
        }
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerComponent)


void CPlayerComponent::ChildInitialize()
{
    m_pCameraComponent = m_pEntity->GetOrCreateComponent< Cry::DefaultComponents::CCameraComponent>();
    m_pInputComponent = m_pEntity->GetOrCreateComponent< Cry::DefaultComponents::CInputComponent>();

    m_pInputComponent->RegisterAction("PlayerMovement", "MoveForward", [this](int activationMode, float value) {m_MovementDirection.y = value; });
    m_pInputComponent->BindAction("PlayerMovement", "MoveForward", eAID_KeyboardMouse, eKI_W);

    m_pInputComponent->RegisterAction("PlayerMovement", "MoveBack", [this](int activationMode, float value) {m_MovementDirection.y = -value; });
    m_pInputComponent->BindAction("PlayerMovement", "MoveBack", eAID_KeyboardMouse, eKI_S);

    m_pInputComponent->RegisterAction("PlayerMovement", "MoveLeft", [this](int activationMode, float value) {m_MovementDirection.x = value; });
    m_pInputComponent->BindAction("PlayerMovement", "MoveLeft", eAID_KeyboardMouse, eKI_A);

    m_pInputComponent->RegisterAction("PlayerMovement", "MoveRight", [this](int activationMode, float value) {m_MovementDirection.x = -value; });
    m_pInputComponent->BindAction("PlayerMovement", "MoveRight", eAID_KeyboardMouse, eKI_D);


    m_pInputComponent->RegisterAction("PlayerRotation", "MouseRotateX", [this](int activationMode, float value) {m_LookDirectionDelta.x = value; });
    m_pInputComponent->BindAction("PlayerRotation", "MouseRotateX", eAID_KeyboardMouse, eKI_MouseX);
    
    m_pInputComponent->RegisterAction("PlayerRotation", "MouseRotateY", [this](int activationMode, float value) {m_LookDirectionDelta.y = value; });
    m_pInputComponent->BindAction("PlayerRotation", "MouseRotateY", eAID_KeyboardMouse, eKI_MouseY);
}

