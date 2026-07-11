#include "stdafx.h"
#include "cPreSpaceColonizationConfig.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"

using namespace SporeModUtils;

cPreSpaceColonizationConfig::cPreSpaceColonizationConfig(ResourceKey preSpaceColonizationConfigKey){
	PropertyListPtr preSpaceColonizationConfig;
	PropManager.GetPropertyList(preSpaceColonizationConfigKey.instanceID, preSpaceColonizationConfigKey.groupID, preSpaceColonizationConfig);

	uint32_t preSpaceColonizationRuleUint;
	App::Property::GetUInt32(preSpaceColonizationConfig.get(), 0xA73FB587, preSpaceColonizationRuleUint);
	preSpaceColonizationRule = PreSpaceColonizationRule(preSpaceColonizationRuleUint);

	App::Property::GetInt32(preSpaceColonizationConfig.get(), 0x6F055754, levelToColonizeTribes);
	App::Property::GetInt32(preSpaceColonizationConfig.get(), 0x5E879D17, levelToColonizeCivilizations);

	App::Property::GetArrayBool(preSpaceColonizationConfig.get(), 0xB2124B2C, tribeColonizationByArchetype);
	App::Property::GetArrayBool(preSpaceColonizationConfig.get(), 0x51A7D023, civilizationColonizationByArchetype);
}


cPreSpaceColonizationConfig::~cPreSpaceColonizationConfig()
{
}

// For internal use, do not modify.
int cPreSpaceColonizationConfig::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cPreSpaceColonizationConfig::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cPreSpaceColonizationConfig::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cPreSpaceColonizationConfig);
	return nullptr;
}

PreSpaceColonizationRule cPreSpaceColonizationConfig::GetPreSpaceColonizationRule(){
	return preSpaceColonizationRule;
}

int cPreSpaceColonizationConfig::GetLevelToColonizeTribes(){
	return levelToColonizeTribes;
}

int cPreSpaceColonizationConfig::GetLevelToColonizeCivilizations(){
	return levelToColonizeCivilizations;
}

bool cPreSpaceColonizationConfig::CanArchetypeColonizeTribes(Simulator::Archetypes archetype){
	return tribeColonizationByArchetype[ArchetypeUtils::GetBaseArchetype(archetype)];
}

bool cPreSpaceColonizationConfig::CanArchetypeColonizeCivilizations(Simulator::Archetypes archetype){
	return civilizationColonizationByArchetype[ArchetypeUtils::GetBaseArchetype(archetype)];;
}
