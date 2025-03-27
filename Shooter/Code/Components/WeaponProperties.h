#pragma once
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/Reflection/TypeDesc.h>


enum  EFireMode {
	AUTOMATIC,
	SEMIAUTOMATIC,
	SINGLEACTION,
	NONE
};
static void ReflectType(Schematyc::CTypeDesc<EFireMode>& desc) {
	desc.SetGUID("{EFED456D-1233-4E12-A86C-54E6DE34CD04}"_cry_guid);
	
	desc.SetLabel("Fire Mode");
	desc.AddConstant(EFireMode::AUTOMATIC, "Automatic", "Automatic");
	desc.AddConstant(EFireMode::SEMIAUTOMATIC, "Semi-Automatic", "Semi-Automatic");
	desc.AddConstant(EFireMode::SINGLEACTION, "Single Action", "Single Action");
	desc.AddConstant(EFireMode::NONE, "None", "None");

	desc.SetDefaultValue(EFireMode::SEMIAUTOMATIC);

}

struct SWeaponProperties
{
	inline bool operator ==(const SWeaponProperties& swp) const { return 0 == memcmp(this, &swp, sizeof(swp)); }
	inline bool operator !=(const SWeaponProperties& swp) const { return 0 != memcmp(this, &swp, sizeof(swp)); }

	Schematyc::GeomFileName sGeometryPath;
	Schematyc::MaterialFileName sMaterialPath;
	EFireMode eFireMode;
	float fShootDelay =0.1;
	
	
};
static void ReflectType(Schematyc::CTypeDesc<SWeaponProperties>& desc) {
	desc.SetGUID("{C8D48268-0667-4313-B794-BD33B64AC0BB}"_cry_guid);
	desc.SetLabel("Weapon Properties");
	desc.SetDescription("Weapon Properties");
	desc.AddMember(&SWeaponProperties::sGeometryPath, 'geom', "geometrypath", "Geometry Path", "Path to geometry file for this weapon", "");
	desc.AddMember(&SWeaponProperties::sMaterialPath, 'mat', "materialpath", "Material Path", "Path to material file for this weapon", "");
	desc.AddMember(&SWeaponProperties::eFireMode, 'frmd', "firemode", "Fire Mode", "Fire mode setting", EFireMode::SEMIAUTOMATIC);
	desc.AddMember(&SWeaponProperties::fShootDelay, 'shdl', "shootdelay", "Shoot Delay", "Delay between the shots", 0.1f);
	
}