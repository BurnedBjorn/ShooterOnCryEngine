#pragma once
#include <StdAfx.h>
#include "Character.h"

#include <CryEntitySystem/IEntitySystem.h>


#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Cameras/CameraComponent.h>
class CPlayerController :public IEntityComponent
{
private:
	static constexpr float DEFAULT_MOUSE_SENSITIVITY = 0.002f;
	static constexpr float DEFAULT_CAMERA_HEIGHT = 2.0f;
	static constexpr float DEFAULT_CAMERA_MAX_ROTATION = 1.5f;
	static constexpr float DEFAULT_CAMERA_MIN_ROTATION = -1.5f;
public:
	CPlayerController() = default;
	virtual ~CPlayerController() {};
	static void ReflectType(Schematyc::CTypeDesc<CPlayerController>& desc)
	{
		desc.SetGUID("{6018EE90-642B-448B-96A6-4E9B456DD4FF}"_cry_guid);
		desc.SetEditorCategory("Characters/Controllers");
		desc.SetLabel("Player Controller");
		desc.SetDescription("Makes Camera and recieves player input. Takes control over Character component or creates it's own");
		desc.AddMember(&CPlayerController::m_MouseSensitivity, 'msns', "mousesensitivity", "Mouse Sensitivity", "Mouse Sensitivity", DEFAULT_MOUSE_SENSITIVITY);
		desc.AddMember(&CPlayerController::m_CameraDefaultHeight, 'cmdh', "camdefh", "Camera Default Height", "Default height of the camera", DEFAULT_CAMERA_HEIGHT);
		desc.AddMember(&CPlayerController::m_CamYRotMax, 'crmx', "cammaxrot", "Camera Max Rotation", "Maximal rotation on y axix", DEFAULT_CAMERA_MAX_ROTATION);
		desc.AddMember(&CPlayerController::m_CamYRotMin, 'crmn', "camminrot", "Camera Min Rotation", "Maximal rotation on y axis", DEFAULT_CAMERA_MIN_ROTATION);
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
	float m_CamYRotMax = 1.5f;
	float m_CamYRotMin = -1.5f;


	void SendMovementUpdate();
	void EntityRotationUpdate();
	void CameraRotationUpdate();
};