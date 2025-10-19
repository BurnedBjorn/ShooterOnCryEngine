#pragma once
#pragma once

#include <StdAfx.h>
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>
#include "WeaponComponent.h"


class CBotComponent :public IEntityComponent
{
private:
	static constexpr float DEFAULT_MOVEMENT_SPEED = 10.0f;
	static constexpr int DEFAULT_HEALTH = 100;
public:
	CBotComponent() = default;
	virtual ~CBotComponent() {};

	static void ReflectType(Schematyc::CTypeDesc<CBotComponent>& desc)
	{
		desc.SetGUID("{{AAB3FF86-4E4A-4776-AFC1-993122A5730D}}"_cry_guid);
		desc.SetEditorCategory("Characters");
		desc.SetLabel("Bot Component");
		desc.SetDescription("Bot");
		desc.AddMember(&CBotComponent::m_MovementSpeed, 'msd', "movementspeed", "Movement Speed", "Character Movement Speed", DEFAULT_MOVEMENT_SPEED);
		desc.AddMember(&CBotComponent::m_Health, 'hlth', "health", "Health", "amount of health points", DEFAULT_HEALTH);
	}

	virtual void Initialize() override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;

	void MovementInput(Vec2 NewInput);
	void RecieveHit(Vec3 Location);
	Vec3 m_MovementDirection;
	Vec2 m_MovementInput;

	//void PickUpWeapon(CWeaponComponent* NewWeapon);
	//void DropWeapon();

	//void Attack(int activationMode);
	//void SetAimTarget(Vec3 NewTarget);


	//void HitDebug(int ipart);
	//void ResetAimPose();

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
	IEntityNavigationComponent* m_pNavigationComponent = nullptr;

	float m_MovementSpeed = 10.0f;

	//inventory will be implemented later
	CWeaponComponent* m_pWeapon = nullptr;

	Vec3 m_AimTarget;

	FragmentID m_movementFragmentID;
	FragmentID m_aimposeFragmentID;
};