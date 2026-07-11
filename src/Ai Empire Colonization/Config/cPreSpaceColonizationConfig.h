#pragma once

#include <Spore\BasicIncludes.h>

#define cPreSpaceColonizationConfigPtr intrusive_ptr<cPreSpaceColonizationConfig>


/// Defines the available rules for colonizing star systems
/// containing pre-space civilizations.
enum class PreSpaceColonizationRule : uint32_t {
	Disabled = 0,
	BasedOnEmpireArchetype = 1,
	Unrestricted = 2
};

/// Stores and provides access to the configuration that determines
/// how empires colonize star systems containing pre-space civilizations.
class cPreSpaceColonizationConfig 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("AiEmpireColonization::cPreSpaceColonizationConfig");
	
	cPreSpaceColonizationConfig(ResourceKey preSpaceColonizationConfigKey);
	~cPreSpaceColonizationConfig();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	// Gets the rule that determines whether pre-space systems can be colonized.
	PreSpaceColonizationRule GetPreSpaceColonizationRule();

	// Gets the minimum empire level required to colonize a star system containing a tribe.
	int GetLevelToColonizeTribes();

	// Gets the minimum empire level required to colonize a star system containing a civilization.
	int GetLevelToColonizeCivilizations();

	// Gets whether the specified empire archetype can colonize systems containing tribes.
	bool CanArchetypeColonizeTribes(Simulator::Archetypes archetype);

	// Gets whether the specified empire archetype can colonize systems containing civilizations.
	bool CanArchetypeColonizeCivilizations(Simulator::Archetypes archetype);

	// Determines whether colonization of systems containing tribes or civilizations is allowed.
	PreSpaceColonizationRule preSpaceColonizationRule;

	// Minimun level for an empire to be able to colonize a star with a tribe.
	int levelToColonizeTribes;

	// Minimun level for an empire to be able to colonize a star with a civ.
	int levelToColonizeCivilizations;

	// Stores whether each empire archetype can colonize systems containing tribes.
	eastl::vector<bool> tribeColonizationByArchetype;

	// Stores whether each empire archetype can colonize systems containing civilizations.
	eastl::vector<bool> civilizationColonizationByArchetype;
};
