#include "StdAfx.h" //Required in all classes to speed up compile
#include "WeaponComponent.h"

#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>
#include <CryAction/IActionMapManager.h>



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
		m_RoundInChamber = true;
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
		switch (event.nParam[0])
		{
		case 'shot':
		{
			switch (m_WeaponProperties.eFireMode)
			{
			case SINGLEACTION:
			{
				CryLog("SingleAction is not yet implemented");
			}
			case SEMIAUTOMATIC:
			{
				m_RoundInChamber = true;
			}
			break;
			case AUTOMATIC:
			{
				m_RoundInChamber = true;
				if (m_Trigger==ETrigger::DOWN)
				{
					Shoot();
				}
			}
			break;
			case NONE:
			{
				m_RoundInChamber = true;
				
			}
			break;
			default:
				break;
			}
		}
		break;
		default:
			break;
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

void CWeaponComponent::Trigger(int activationMode)
{
	
	switch (activationMode)
	{

	case eAAM_OnPress:
	{
		
		m_Trigger = ETrigger::DOWN;
		Shoot();
	}
	break;
	case eAAM_OnRelease:
	{
		m_Trigger = ETrigger::UP;
	}
	break;
	default:
		break;
	}
}

void CWeaponComponent::SetTarget(Vec3 NewTarget)
{
	m_Target = NewTarget;
}

ray_hit CWeaponComponent::Raycast(Vec3 Start, Vec3 End)
{
	Vec3 Direction = End - Start;
	ray_hit Hit;
	static const unsigned int RayFlags = rwi_stop_at_pierceable | rwi_colltype_any;
	gEnv->pPhysicalWorld->RayWorldIntersection(Start, Direction, ent_all, RayFlags, &Hit, 1, m_pEntity->GetPhysicalEntity());
	return Hit;
}

void CWeaponComponent::Shoot()
{
	
	if (m_RoundInChamber)
	{
		Vec3 RayEnd = m_Target - GetBarrelWorldPos();
		RayEnd.NormalizeSafe();
		RayEnd *= 1000;
		RayEnd += GetBarrelWorldPos();

		ray_hit Hit = Raycast(GetBarrelWorldPos(), RayEnd);
		if (IPhysicalEntity* pPhysEntity = Hit.pCollider)
		{
			if (IEntity* pEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pPhysEntity))
			{
				if (CCharacterComponent* pCharacterComponent = pEntity->GetComponent<CCharacterComponent>())
				{
					pCharacterComponent->HitDebug();
				}
			}
		}



		Vec3 AttackTarget = Hit.pt;
		gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(AttackTarget - Vec3(0, 0, 0.5), ColorB(255, 0, 0), AttackTarget + Vec3(0, 0, 0.5), ColorB(255, 0, 0), 6.0f);
		gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(AttackTarget - Vec3(0, 0.5, 0), ColorB(255, 0, 0), AttackTarget + Vec3(0, 0.5, 0), ColorB(255, 0, 0), 6.0f);
		gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(AttackTarget - Vec3(0.5, 0, 0), ColorB(255, 0, 0), AttackTarget + Vec3(0.5, 0, 0), ColorB(255, 0, 0), 6.0f);

		m_RoundInChamber = false;
		m_pEntity->SetTimer(this, m_pEntity->GetId(), "{4976AF5F-3077-46FD-ABD8-ADD14DF6CB5A}"_cry_guid, 'shot', int(m_WeaponProperties.fShootDelay * 1000));
	}
}



