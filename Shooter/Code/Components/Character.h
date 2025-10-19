#pragma once

#include <StdAfx.h>
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include "WeaponComponent.h"


class CCharacterComponent :public IEntityComponent
{
private:
	static constexpr float DEFAULT_MOVEMENT_SPEED = 10.0f;
	static constexpr int DEFAULT_HEALTH = 100;
public:
	CCharacterComponent() = default;
	virtual ~CCharacterComponent() {};

	static void ReflectType(Schematyc::CTypeDesc<CCharacterComponent>& desc)
	{
		desc.SetGUID("{0927A07B-F565-41F8-A530-230116291B1D}"_cry_guid);
		desc.SetEditorCategory("Characters");
		desc.SetLabel("Base Character Component");
		desc.SetDescription("Character component that can be controlled with a controller");
		desc.AddMember(&CCharacterComponent::m_MovementSpeed, 'msd', "movementspeed", "Movement Speed", "Character Movement Speed", DEFAULT_MOVEMENT_SPEED);
		desc.AddMember(&CCharacterComponent::m_Health, 'hlth', "health", "Health", "amount of health points", DEFAULT_HEALTH);
	}

	virtual void Initialize() override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	
	virtual Cry::Entity::EventFlags GetEventMask() const override;

	void MovementInput(Vec2 NewInput);

	Vec3 m_MovementDirection; 
	Vec2 m_MovementInput;
	
	void PickUpWeapon(CWeaponComponent* NewWeapon);
	void DropWeapon();

	void Attack(int activationMode);
	void SetAimTarget(Vec3 NewTarget);
	

	void HitDebug(int ipart);
	void ResetAimPose();

	void AnimationUpdate();
	
	
	enum BodyPart {
		HEAD, BODY, ARMS, LEGS, OUTOFRANGE
	};
	BodyPart IndexToBodyPart(int index) {
		switch (index)
		{
		case 0:
		case 1:
		case 4:
		case 5:
		case 6:
			return BODY;
			break;
		case 7:
			return HEAD;
			break;
		case 2:
		case 3:
		case 12:
		case 13:
			return LEGS;
			break;
		case 8:
		case 9:
		case 10:
		case 11:
			return ARMS;
			break;
		default:
			return OUTOFRANGE;
		}
	}
protected:
	int m_Health = 100;
	
	
private:
	Cry::DefaultComponents::CStaticMeshComponent* m_pStaticMesh;
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAdvancedAnimationController;

	float m_MovementSpeed = 10.0f;

	//inventory will be implemented later
	CWeaponComponent* m_pWeapon = nullptr;

	Vec3 m_AimTarget;

	FragmentID m_movementFragmentID;
	FragmentID m_aimposeFragmentID;
};