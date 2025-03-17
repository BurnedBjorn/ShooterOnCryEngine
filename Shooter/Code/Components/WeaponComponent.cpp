#include "StdAfx.h" //Required in all classes to speed up compile
#include "WeaponComponent.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>



// Makes the component 'editable' within the CE editor. 
static void RegisterWeaponComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CWeaponComponent));
	}
}


CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWeaponComponent)

void CWeaponComponent::Initialize()
{
	m_pEntity->LoadGeometry(WEAPON, "Assets/Objects/Weapons/Test/assets.cgf");
	

}

Cry::Entity::EventFlags CWeaponComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void CWeaponComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Update:
	{
		
		Vec3 BarrelLocation =GetBarrelWorldPos();
		
		
	}
	break;
	default:
		break;
	}
}

Vec3 CWeaponComponent::GetBarrelWorldPos()
{
	if (m_pEntity)
	{
		return m_pEntity->GetStatObj(WEAPON)->GetHelperPos("Barrel")+m_pEntity->GetWorldPos();
	}
	return Vec3(0,0,0);
}

void CWeaponComponent::CheckBarrelLocation(Vec3 Location)
{
	gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(Location - Vec3(0, 0, 0.5), ColorB(0, 0, 255), Location + Vec3(0, 0, 0.5), ColorB(0, 0, 255), 6.0f);
	gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(Location - Vec3(0, 0.5, 0), ColorB(0, 0, 255), Location + Vec3(0, 0.5, 0), ColorB(0, 0, 255), 6.0f);
	gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(Location - Vec3(0.5, 0, 0), ColorB(0, 0, 255), Location + Vec3(0.5, 0, 0), ColorB(0, 0, 255), 6.0f);
	CryLog("%f,%f,%f", Location.x, Location.y, Location.z);
}
