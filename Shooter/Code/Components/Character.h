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
		desc.AddMember(&CCharacterComponent::m_RotationSpeed, 'rsd', "rotationspeed", "Rotation Speed", "Character Rotation Speed", 0.5f);
		desc.AddMember(&CCharacterComponent::m_rotationYMin, 'ryn', "rotationymin", "Y rotation minimum", "How much can the character look down", -0.84f);
		desc.AddMember(&CCharacterComponent::m_rotationYMax, 'ryx', "rotationymax", "Y rotation maximum", "How much can the character look up", 1.5f);
	}

	virtual void Initialize() override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	
	virtual Cry::Entity::EventFlags GetEventMask() const override;

	virtual void ChildInitialize() {};
	
protected:

	Vec2 m_MovementDirection; //Direction of movement relative to character's rotation
	void MovementUpdate(const float frameTime);

	Vec2 m_LookDirectionDelta; //For aiming, character rotation and for camera 
	void RotaionUpdate(const float frameTime);
	
private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAdvancedAnimationController;
	float m_MovementSpeed = 20.0f;

	float m_RotationSpeed = 0.5f;

	float m_rotationYMin = -0.84f;
	float m_rotationYMax = 1.5f;
};