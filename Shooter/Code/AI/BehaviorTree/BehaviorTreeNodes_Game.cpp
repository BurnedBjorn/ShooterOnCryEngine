//#include "BehaviorTreeNodes_Game.h"

// Copyright 2012-2021 Crytek GmbH / Crytek Group. All rights reserved.
 
#include "StdAfx.h"
#include "BehaviorTreeNodes_Game.h"
#include <CryAISystem/BehaviorTree/IBehaviorTree.h>
#include <CryAISystem/BehaviorTree/Action.h	>
#include <CryAISystem/BehaviorTree/Node.h>
#include <CryAISystem/BehaviorTree/Decorator.h>
#include <CryAISystem/IAISystem.h>
#include <Components/AIController.h>
//#include "../Agent.h"
//#include "../AISquadManager.h"
//#include "../GameAISystem.h"
//#include "GameRules.h"
//#include "ActorImpulseHandler.h"
//#include "Actor.h"
//#include "IAnimatedCharacter.h"
#include <CrySystem/XML/XMLAttrReader.h>
//#include "PlayerAnimation.h"
//#include "GamePhysicsSettings.h"

//#ifdef _DEBUG
//# define INCLUDE_KEEP_TARGET_AT_A_DISTANCE_DEBUG_CODE
//# define INCLUDE_MELEE_DEBUG_CODE
//#endif
		 /*
namespace
{
	IEntity* GetEntity(const BehaviorTree::UpdateContext& updateContext)
	{
		return gEnv->pEntitySystem->GetEntity(updateContext.entityId);
	}

	IAIActor* GetIAIActor(const BehaviorTree::UpdateContext& updateContext)
	{
		if (IEntity* entity = GetEntity(updateContext))
		{
			if (IAIObject* ai = entity->GetAI())
				return ai->CastToIAIActor();
		}

		return NULL;
	}
}
*/ 
namespace BehaviorTree
{
	class TestLog : public Action
	{
		typedef Action BaseClass;
	public:
		struct RuntimeData {};
		virtual void OnInitialize(const UpdateContext& context)
		{
			BaseClass::OnInitialize(context);
			

		}
		virtual Status Update(const UpdateContext& context)
		{
			gEnv->pLog->Log("TestBTLog");
			return Success;
		}
		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const struct LoadContext& context, const bool isLoadingFromEditor) override
		{
			//IF_UNLIKELY(BaseClass::LoadFromXml(xml, context) == LoadFailure)
				//return LoadFailure;

			// No attributes to load for this simple node
			if (BaseClass::LoadFromXml(xml, context, isLoadingFromEditor) == LoadFailure)
			{
				return LoadFailure;
			}

			const char* Name = xml->getAttr("name");
			CryLogAlways(Name);
			return LoadSuccess;

		}
	};
	
	class TestVar : public Action
	{
		typedef Action BaseClass;
	public:
		struct RuntimeData {
			bool RecievedEvent;
			RuntimeData() { RecievedEvent = false; };
		};
		virtual void OnInitialize(const UpdateContext& context)
		{
			BaseClass::OnInitialize(context);


		}
		virtual Status Update(const UpdateContext& context)
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			if (runtimeData.RecievedEvent)
			{
				return Success;
			}	
			//context.entity.GetCharacter(1)->GetFilePath();
			bool test = false;
			context.blackboard.GetVariable("Log", test);

			IAIObject* pAiObject;
			ICharacterInstance* character;
			CAIController* AiController;
			//EntityId test = context.entityId;
			//int a = test;
			for (int i = 0; i < 16; i++)
			{
				if (character = context.entity.GetCharacter(i))
				{
					gEnv->pLog->Log("%i%",int(context.entityId));
					break;
				}
				
			}
			if (AiController =context.entity.GetComponent<CAIController>())
			{
				gEnv->pLog->Log(AiController->GetCode());
				//AiController->Forward();
			}
			else {
				gEnv->pLog->Log("No AiController");
			}
			if (pAiObject = context.entity.GetAI())
			{

				pAiObject;
			}
			else if (!pAiObject)
			{
				
			}
			if (test)
			{
				gEnv->pLog->Log("test");
			}
			
			
			return Running;
		}
		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const struct LoadContext& context, const bool isLoadingFromEditor) override
		{
			//IF_UNLIKELY(BaseClass::LoadFromXml(xml, context) == LoadFailure)
				//return LoadFailure;

			// No attributes to load for this simple node
			if (BaseClass::LoadFromXml(xml, context, isLoadingFromEditor) == LoadFailure)
			{
				return LoadFailure;
			}

			const char* Name = xml->getAttr("name");
			CryLogAlways(Name);
			return LoadSuccess;

		}
		virtual void HandleEvent(const EventContext& context, const Event& event) override
		{
			RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			runtimeData.RecievedEvent = true;
			gEnv->pLog->Log(event.GetName());
		}
		
	private:
		
	};

	class TestSwitch : public Decorator {
		typedef Decorator BaseClass;
	public:	  
		struct RuntimeData
		{
			bool gateIsOpen;
			RuntimeData() :gateIsOpen(false) {};
		};
		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const struct LoadContext& context, const bool strictMode) override
		{					 
			return	BaseClass::LoadFromXml(xml, context, strictMode);
		}
		virtual void OnInitialize(const UpdateContext& context) override
		{
		}
		virtual Status Update(const UpdateContext& context) override
		{
			bool open;
			
			if (context.blackboard.GetVariable("shot", open))
			{
				if (open)
				{
					//gEnv->pLog->Log("ShotVariableIsTrue");
					return BaseClass::Update(context);
				}
				//gEnv->pLog->Log("shot vairable is false");
				return Failure;
			}
			gEnv->pLog->LogError("Variable shot was not found in blackboard");
			return Failure;
		}
	private:
	};

	class PrintDistance : public Action
	{
		typedef Action BaseClass;
	public:
		struct RuntimeData {};
		virtual void OnInitialize(const UpdateContext& context)
		{
			BaseClass::OnInitialize(context);


		}
		virtual Status Update(const UpdateContext& context)
		{
			float distance;
			if (context.blackboard.GetVariable("distance", distance))
			{
				gEnv->pLog->Log("%f%", distance);
				return Success;
			}
			gEnv->pLog->LogError("DistanceVariableNotFound");
			return Failure;
		}
		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const struct LoadContext& context, const bool isLoadingFromEditor) override
		{
			//IF_UNLIKELY(BaseClass::LoadFromXml(xml, context) == LoadFailure)
				//return LoadFailure;

			// No attributes to load for this simple node
			if (BaseClass::LoadFromXml(xml, context, isLoadingFromEditor) == LoadFailure)
			{
				return LoadFailure;
			}

			const char* Name = xml->getAttr("name");
			CryLogAlways(Name);
			return LoadSuccess;

		}
	};

}

void RegisterGameBehaviorTreeNodes()
{
	using namespace BehaviorTree;

	assert(gEnv->pAISystem->GetIBehaviorTreeManager());
	
	IBehaviorTreeManager& manager = *gEnv->pAISystem->GetIBehaviorTreeManager();
	//gEnv->pLog->Log("Registering TestLog node AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA Registering TestLog node AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA Registering TestLog node AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA Registering TestLog node AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA Registering TestLog node AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	REGISTER_BEHAVIOR_TREE_NODE(manager, TestLog);		
	REGISTER_BEHAVIOR_TREE_NODE(manager, TestVar);
	REGISTER_BEHAVIOR_TREE_NODE(manager, TestSwitch);
	REGISTER_BEHAVIOR_TREE_NODE(manager, PrintDistance);
	//gEnv->pLog->Log("TestLog node registered successfully");
	
	
	//REGISTER_BEHAVIOR_TREE_NODE(manager, HoldFormation);
	//REGISTER_BEHAVIOR_TREE_NODE(manager, JoinFormation);
	//REGISTER_BEHAVIOR_TREE_NODE(manager, Melee);
	//REGISTER_BEHAVIOR_TREE_NODE(manager, KeepTargetAtADistance);
	//REGISTER_BEHAVIOR_TREE_NODE(manager, SuppressHitReactions);
	//REGISTER_BEHAVIOR_TREE_NODE(manager, InflateAgentCollisionRadiusUsingPhysicsTrick);
}