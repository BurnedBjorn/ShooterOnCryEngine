/*
	BaseClass for gun
*/
#pragma once

#include <CryEntitySystem/IEntitySystem.h>
#include <CryGame/IGameFramework.h>
#include "Character.h"


class CWeaponComponent : public IEntityComponent // Best practice: Classes start with a 'C'

{
public:
	// Provide a virtual destructor, ensuring correct destruction of IEntityComponent members
	virtual ~CWeaponComponent() = default;

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox

	static void ReflectType(Schematyc::CTypeDesc<CWeaponComponent>& desc)
	{
		// Provide a globally unique identifier for the component, can be generated in Visual Studio via Tools -> Create GUID (in registry format).
		desc.SetGUID("{9877D70E-3FD6-4721-90F5-1B41FC93A496}"_cry_guid);
		// These set the label and categogory in the editor interface. 
		desc.SetEditorCategory("Weapons");
		desc.SetLabel("WeaponComponent");
	}
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	Vec3 GetBarrelWorldPos();
	void CheckBarrelLocation(Vec3 Location);
protected:
	
private:
	CCharacterComponent* m_pOwner = nullptr;
	enum EGeometrySlots {
		WEAPON = 0
	};
	
};
