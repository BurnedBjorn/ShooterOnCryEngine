#pragma once

#include <StdAfx.h>
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>


class CCharacterComponent :public IEntityComponent
{
public:
	CCharacterComponent() = default;
	virtual ~CCharacterComponent() {};

	static void ReflectType(Schematyc::CTypeDesc<CCharacterComponent>& desc)
	{
		desc.SetGUID("{0927A07B-F565-41F8-A530-230116291B1D}"_cry_guid);
		desc.SetEditorCategory("Characters");
		desc.SetLabel("Base Character Component");
		desc.SetDescription("Character component that can be controlled with a controller");
		desc.AddMember(&CCharacterComponent::m_MovementSpeed, 'msd', "movementspeed", "Movement Speed", "Character Movement Speed", 20.0f);
	}

	virtual void Initialize() override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	
	virtual Cry::Entity::EventFlags GetEventMask() const override;

	

	Vec2 m_MovementDirection; //Direction of movement relative to character's rotation
	Vec2 m_LookInput;
protected:

	
	
private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAdvancedAnimationController;

	float m_MovementSpeed = 20.0f;


	void MovementUpdate();
	Quat m_LookOrientation;

	bool m_Gameplay = false;
};