#pragma once
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/Reflection/TypeDesc.h>
struct SWeaponProperties
{
	inline bool operator ==(const SWeaponProperties& swp) const { return 0 == memcmp(this, &swp, sizeof(swp)); }
	inline bool operator !=(const SWeaponProperties& swp) const { return 0 != memcmp(this, &swp, sizeof(swp)); }

	Schematyc::GeomFileName sGeometryPath;
	Schematyc::MaterialFileName sMaterialPath;
};
static void ReflectType(Schematyc::CTypeDesc<SWeaponProperties>& desc) {
	desc.SetGUID("{C8D48268-0667-4313-B794-BD33B64AC0BB}"_cry_guid);
	desc.SetLabel("Weapon Properties");
	desc.SetDescription("Weapon Properties");
	desc.AddMember(&SWeaponProperties::sGeometryPath, 'geom', "geometrypath", "Geometry Path", "Path to geometry file for this weapon", "");
	desc.AddMember(&SWeaponProperties::sMaterialPath, 'mat', "materialpath", "Material Path", "Path to material file for this weapon", "");
}