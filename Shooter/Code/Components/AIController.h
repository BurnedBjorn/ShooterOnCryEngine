#pragma once
#include <StdAfx.h>

#include "Character.h"
#include "WeaponComponent.h"



#include <CryAISystem/Components/IEntityNavigationComponent.h>
#include <CryEntitySystem/IEntitySystem.h>


class CAIController :public IEntityComponent
{
public:
	CAIController() = default;
	~CAIController() {};
	static void ReflectType(Schematyc::CTypeDesc<CAIController>& desc){
		desc.SetGUID("{{785056B0-F504-47AA-BB8B-3FC6165AF511}}"_cry_guid);
		desc.SetEditorCategory("Characters/Controllers");
		desc.SetLabel("AI Controller");
		desc.SetDescription("Should automatically controll character");
	}
	virtual void Initialize() override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	void RecieveHit(Vec3 Location);
private:
	CCharacterComponent* m_pControlledCharacter = nullptr;
	IEntityNavigationComponent* m_pNavigationComponent = nullptr;
	Vec3 m_recommendedVelocity;
};