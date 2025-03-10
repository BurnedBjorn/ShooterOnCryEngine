#pragma once
#include <StdAfx.h>
#include <CryEntitySystem/IEntityComponent.h>
class CPlayerController :public IEntityComponent
{
public:
	CPlayerController() = default;
	virtual ~CPlayerController() {};
	static void ReflectType(Schematyc::CTypeDesc<CPlayerController>& desc)
	{
		desc.SetGUID("{6018EE90-642B-448B-96A6-4E9B456DD4FF}"_cry_guid);
		desc.SetEditorCategory("Characters");
		desc.SetLabel("Base Character Component");
		desc.SetDescription("Character component that can be controlled with a controller");
		
	}
protected:
private:
};