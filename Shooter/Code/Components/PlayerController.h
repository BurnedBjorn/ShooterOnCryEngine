#pragma once
#include <StdAfx.h>
#include "Character.h"

#include <CryEntitySystem/IEntityComponent.h>


#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Cameras/CameraComponent.h>
class CPlayerController :public IEntityComponent
{
public:
	CPlayerController() = default;
	virtual ~CPlayerController() {};
	static void ReflectType(Schematyc::CTypeDesc<CPlayerController>& desc)
	{
		desc.SetGUID("{6018EE90-642B-448B-96A6-4E9B456DD4FF}"_cry_guid);
		desc.SetEditorCategory("Characters/Controllers");
		desc.SetLabel("Player Controller");
		desc.SetDescription("Makes Camera and recieves player input. Takes control over Character component or creates it's own");
		desc.AddMember(&CPlayerController::m_MouseSensitivity, 'msns', "mousesensitivity", "Mouse Sensitivity", "Mouse Sensitivity", 0.002f);
		desc.AddMember(&CPlayerController::m_CameraDefaultHeight, 'cmdh', "camdefh", "Camera Default Height", "Default height of the camera", 2);
	}

	virtual void Initialize() override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;

	void InitializeInput();
protected:
	
private:
	CCharacterComponent* m_pControlledCharacter = nullptr;
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent= nullptr;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;

	Vec2 m_MovementInput;
	Vec2 m_LookInput;
	Quat m_EntityOrientation;
	Matrix34 m_CameraTransform;

	float m_MouseSensitivity=0.002f;
	float m_CameraDefaultHeight =2;

	void SendMovementUpdate();
	void EntityRotationUpdate();
	void CameraRotationUpdate();
};