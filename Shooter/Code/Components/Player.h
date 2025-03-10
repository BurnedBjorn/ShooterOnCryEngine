#pragma once
#include <StdAfx.h>
#include "Character.h"
#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Input/InputComponent.h>

class CPlayerComponent :public CCharacterComponent
{
public:
	CPlayerComponent() = default;
	virtual ~CPlayerComponent() {};

	static void ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc)
	{
		desc.SetGUID("{B5A1CEA9-021D-43CE-98EC-B3E4941AA7A9}"_cry_guid);
		desc.SetEditorCategory("Characters/Player");
		desc.SetLabel("Player Character Component");
		desc.SetDescription("Character component with camera and input components");
	}

	virtual void ChildInitialize() override;

	
	
protected:
private:
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent;
};