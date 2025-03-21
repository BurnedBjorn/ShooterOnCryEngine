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
	SetPhysics(true);
	m_DefaultLocation = m_pEntity->GetWorldPos();
	CryLog("Initialize");

	m_RoundInChamber = true;
	
}

Cry::Entity::EventFlags CWeaponComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::Update|Cry::Entity::EEvent::Reset;
}

void CWeaponComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::Update:
	{
		
		Vec3 BarrelLocation =GetBarrelWorldPos();
		CheckBarrelLocation(GetBarrelWorldPos());
		
	}
	break;
	case Cry::Entity::EEvent::Reset:
	{
		if (HasOwner())
		{
			SetPhysics(false);
		}
		else
		{
			SetPhysics(true);
			Matrix34 NewTM = m_pEntity->GetWorldTM();
			NewTM.SetTranslation(m_DefaultLocation);
			m_pEntity->SetWorldTM(NewTM);
		}
		
	}

	break;
	case Cry::Entity::EEvent::TimerExpired:
	{
		int id = int(event.nParam[0]);
		CryLog("%d", id);
		if (event.nParam[0]== 'atk')
		{
			CryLog("Timer balls");
			m_RoundInChamber = true;
		}
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
		IStatObj* pStatObj = m_pEntity->GetStatObj(WEAPON);
		if (pStatObj)
		{
			Vec3 localBarrelPos = pStatObj->GetHelperPos("Barrel");
			return m_pEntity->GetWorldTM().TransformPoint(localBarrelPos); // Correctly transform to world space
		}
	}
	return Vec3(0, 0, 0); // Default return if something fails
}

void CWeaponComponent::CheckBarrelLocation(Vec3 Location)
{
	gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(Location - Vec3(0, 0, 0.5), ColorB(0, 0, 255), Location + Vec3(0, 0, 0.5), ColorB(0, 0, 255), 6.0f);
	gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(Location - Vec3(0, 0.5, 0), ColorB(0, 0, 255), Location + Vec3(0, 0.5, 0), ColorB(0, 0, 255), 6.0f);
	gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(Location - Vec3(0.5, 0, 0), ColorB(0, 0, 255), Location + Vec3(0.5, 0, 0), ColorB(0, 0, 255), 6.0f);
	//CryLog("%f,%f,%f", Location.x, Location.y, Location.z);
}

void CWeaponComponent::PickUp(CCharacterComponent* pNewOwner)
{
	if (!HasOwner())
	{
		m_pOwner = pNewOwner;
		SetPhysics(false);
	}
}

void CWeaponComponent::Drop()
{
	Matrix34 NewTM = m_pEntity->GetWorldTM();
	m_pEntity->DetachThis();
	m_pOwner = nullptr;
	m_pEntity->SetWorldTM(NewTM);
	SetPhysics(true);
	pe_action_impulse impulse;
	impulse.impulse = m_pEntity->GetWorldRotation()*Vec3(0, 5, 0);
	if (m_pEntity->GetPhysicalEntity())
	{
		m_pEntity->GetPhysicalEntity()->Action(&impulse);
	}
	
}

void CWeaponComponent::SetPhysics(bool on)
{
	SEntityPhysicalizeParams params;
	if (on)
	{
		params.type = PE_RIGID;
		//CryLog("Physics ON");
	}
	else
	{
		params.type = PE_NONE;
		//CryLog("PhysicsOFF");
	}
	params.mass = m_mass;
	m_pEntity->Physicalize(params);
}

void CWeaponComponent::TriggerTest()
{
	if (m_RoundInChamber)
	{
		CryLog("Attack");
		m_RoundInChamber = false;
		m_pEntity->SetTimer(this, GetEntityId(),m_pEntity->GetGuid(),'atk',1000);
	}
}

