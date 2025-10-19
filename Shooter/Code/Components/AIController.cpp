#include "AIController.h"

#include <CryAISystem/IAISystem.h>
#include <CryAISystem/INavigation.h>

#include <DefaultComponents/Physics/CharacterControllerComponent.h>
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


static void RegisterAIControllerComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CAIController));
        {
        }
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAIControllerComponent)


void CAIController::Initialize() 
{
    //initialize Controlled Character
    m_pControlledCharacter = m_pEntity->GetOrCreateComponent<CCharacterComponent>();
    if (!m_pControlledCharacter)
    {
        CryLog("No Character component");
    }
    else
    {
        CryLog("Character Component Loaded");
    }
    //
    //initialize Behaviour tree
    m_pBehaviourTree = m_pEntity->GetOrCreateComponent<IEntityBehaviorTreeComponent>();
    if (m_pBehaviourTree)
    {
        
    }
    else
    {
        CryLog("m_pBehaviourTree not initialized");
    }
    //
    //initialize Cover User
    m_pCoverUser = m_pEntity->GetOrCreateComponent<IEntityCoverUserComponent>();
    if (m_pCoverUser)
    {

    }
    else
    {
        CryLog("m_pCoverUser not initialized");

    }
    //
    //initialize Faction Component
    m_pFactionComponent = m_pEntity->GetOrCreateComponent<IEntityFactionComponent>();
    if (m_pFactionComponent)
    {

    }
    else
    {
        CryLog("m_pFactionComponent not initialized");

    }
    //
    //initialize Listener Component
    m_pListenerComponent = m_pEntity->GetOrCreateComponent<IEntityListenerComponent>();
    if (m_pListenerComponent)
    {

    }
    else
    {
        CryLog("m_pListenerComponent not initialized");

    }
    //
    //initialize Navigation Component
    m_pNavigationComponent = m_pEntity->GetOrCreateComponent<IEntityNavigationComponent>();
    if (m_pNavigationComponent)
    {
        m_pNavigationComponent->SetNavigationAgentType("MediumSizedCharacters");
        IEntityNavigationComponent::SMovementProperties MovementProperties;
        MovementProperties.normalSpeed = 10;
        MovementProperties.maxSpeed = 11;
        MovementProperties.maxAcceleration = 15;

        m_pNavigationComponent->SetMovementProperties(MovementProperties);
        //IEntityNavigationComponent::StateUpdatedCallback callback;
        

        m_pNavigationComponent->SetStateUpdatedCallback([this](const Vec3& recommendedVelocity) {

            
            //m_pCharacterController->ChangeVelocity(recommendedVelocity, Cry::DefaultComponents::CCharacterControllerComponent::EChangeVelocityMode::SetAsTarget);
            m_pControlledCharacter->MovementInput(Vec2(recommendedVelocity.x, recommendedVelocity.y));
            }) ;
    }
    //
    //intitalize Observer Component
    m_pObserverComponent = m_pEntity->GetOrCreateComponent<IEntityObserverComponent>();
    if (m_pObserverComponent)
    {

    }
    else
    {
        CryLog("m_pObserverComponent not initialized");

    }
    //
};
void CAIController::ProcessEvent(const SEntityEvent& event) 
{
    switch (event.event)
    {
        
    case Cry::Entity::EEvent::GameplayStarted:
    {
        //m_pControlledCharacter->MovementInput(Vec2(1, 0));
        //CryLog("Begin");
        break;
    }
    case Cry::Entity::EEvent::Reset:
    {
        break;
    }
    case Cry::Entity::EEvent::Update:
    {
        m_pNavigationComponent->NavigateTo(gEnv->pEntitySystem->FindEntityByName("Player")->GetWorldPos());
        break;
    }
    case Cry::Entity::EEvent::TimerExpired:
    {
        //switch (event.nParam[0])
        //{
        //default:
        //    break;
        //}
        
    }
    default:
        break;
    }
};

Cry::Entity::EventFlags CAIController::GetEventMask() const
{
    return Cry::Entity::EEvent::Update |
        Cry::Entity::EEvent::GameplayStarted |
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::TimerExpired;
}
void CAIController::RecieveHit(Vec3 Location)
{
    //Vec3 Location;
    
    m_pNavigationComponent->NavigateTo(Location);
   
    CryLog("%i; %i; %i;", Location.x,Location.y, Location.z);
}
//Cry::Entity::EventFlags CAIController::GetEventMask() const {};
