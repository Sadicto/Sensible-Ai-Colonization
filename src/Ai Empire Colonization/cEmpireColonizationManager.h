#pragma once

#include <Spore\BasicIncludes.h>
#include <Spore/Simulator/Serialization.h>
#include "Spore-Mod-Utils/Include/SporeModUtils.h"

#define cEmpireColonizationManagerPtr intrusive_ptr<cEmpireColonizationManager>

using namespace Simulator;
using namespace SporeModUtils;

class cEmpireColonizationManager
	: public Simulator::cStrategy
{
public:

	enum class PlanetColonizationConfig : uint32_t {
		Disabled = 0,
		OnlyHabitable = 1,
		AllPlanets = 2
	};

	static const uint32_t TYPE = id("AiEmpireColonization::cEmpireColonizationManager");
	static const uint32_t NOUN_ID = TYPE;

	int AddRef() override;

	int Release() override;

	void Initialize() override;

	void Dispose() override;

	const char* GetName() const override;

	bool Write(Simulator::ISerializerStream* stream) override;

	bool Read(Simulator::ISerializerStream* stream) override;

	bool WriteToXML(Simulator::XmlSerializer* xml) override;

	void Update(int deltaTime, int deltaGameTime) override;
	// Inherited via cStrategy

	void OnModeEntered(uint32_t previousModeID, uint32_t newModeID) override;

	static Simulator::Attribute ATTRIBUTES[];
	//
	// You can add more methods here
	//

	static cEmpireColonizationManagerPtr Get();

	/**
	 * @brief Calculates the colonization score of a planet,
	 * with higher scores for a higher terrascore,
	 * expensive spice, and T0 with green orbits,
	 * and lower scores for moons.
	 * Preconditions: none.
	 * @param planet Pointer to the planet (cPlanetRecord*) to evaluate.
	 * @return float score representing the colonization potential of the planet.
	 */
	float PlanetColonizationScore(cPlanetRecord* planet);

	/**
	* @brief Determines if a given star system is colonizable, meaning it is a star,
	* has no owning empire (tribes and civilizations do not count), has no monolith
	* is not a save, not sol, contains at least one colonizable planet and
	* is at most activeRadius parsecs from the player
	* Preconditions: none.
	* @param star Pointer to the star system (cStarRecord*) to check for colonization.
	* @return true if the star is colonizable, false otherwise.
	*/
	bool ColonizableStar(cStarRecord* star);


	/**
	 * Determines whether the given empire can colonize the specified star system.
	 * A star is colonizable if ColonizableStar(star) returns true and,
	 * if the system contains civilizations or tribes, the empire must have a high enough level.
	 */
	bool EmpireCanColonizeStar(cEmpire* empire, cStarRecord* star);

	/**
	 * @brief Finds the planet with the highest colonization score in a star.
	 * Preconditions: At least one planet in the star, ColonizablePlanet(planet).
	 * @param star Pointer to the star system (cStarRecord*) to evaluate.
	 * @return Pointer to the best colonizable planet (cPlanetRecordPtr).
	 */
	cPlanetRecordPtr BestColonizablePlanet(cStarRecord* star);

	/**
	 * @brief Colonizes the planet with the given empire. The colonized planet will have its ecosystem filled.
	 * Preconditions: No tribes or civilization planets.
	 * @param empire
	 * @param planet
	 */
	void ColonizePlanet(cEmpire* empire, cPlanetRecord* planet);

	/**
	 * @brief Calculates the colonization score of a star system
	 * using its highest planet score, reducing it if the star
	 * has tribes and further if it has a civilization.
	 * Preconditions: At least one planet in the star, ColonizablePlanet(planet).
	 * @param star Pointer to the star system (cStarRecord*) to evaluate.
	 * @return Float score representing the colonization potential of the star.
	 */
	float StarColonizationScore(cStarRecord* star);

	/**
	 * @brief Colonizes a star system with the given empire. The colonized planet will have its ecosystem filled.
	 * Preconditions: ColonizableStar(star) and no tribes or civilizations present.
	 * @param empire Pointer to the empire (cEmpire*) performing the colonization.
	 * @param star Pointer to the star system (cStarRecord*) being colonized.
	 */
	void ColonizeStarSystem(cEmpire* empire, cStarRecord* star);

	/**
	 * @brief Colonizes the best available planet in the empire's owned systems.
	 * If no planets are available, no colonization occurs.
	 * @param empire.
	 */
	void ColonizePlanetInOwnedSystem(cEmpire* empire);

	/**
	* @brief Expands the empire to a new star system,
	* prioritizing those closer to the homeworld, other colonies,
	* and a higher StarColonizationScore(Star).
	* The empire may not expand if there are no nearby colonizable stars.
	* Will never colonize GetCurrentStar().
	* Preconditions: none.
	* @param empire Pointer to the empire (cEmpire*) expanding.
	*/
	void ExpandEmpire(cEmpire* empire);

	/**
	 * @brief Calculates the probability of an empire colonizing a new system
	 * Preconditions: none.
	 * @param empire Pointer to the empire (cEmpire*) being evaluated.
	 * @return Float between 0 and 1 representing the probability of colonization.
	 */
	float EmpireColonizationProbability(cEmpire* empire);

	/**
	 * @brief Executes one cycle of the manager, expanding nearby empires
	 * according to EmpireColonizationProbability(empire).
	 * Preconditions: none.
	 */
	void EmpiresExpansionCycle();
	

private:

	
	static cEmpireColonizationManagerPtr instance;

	// Radius (in parsecs) in which empires colonize systems
	float activeRadius;

	// Miliseconds of gameTime between expansion cycles
	int cycleInterval;

	// Number of systems for which the probabilities are calculated from.
	float targetNumSystems;

	// Average cycles necessary for an empire to reach targetNumSystems colonies.
	float cyclesToTargetColonies;
	
	// Colonization range per empire level, in parsecs.
	eastl::vector<float> colonizationRange;

	// Minimun level for an empire to be able to colonize a star with a tribe.
	int levelToColonizeTribe;

	// Minimun level for an empire to be able to colonize a star with a civ.
	int levelToColonizeCiv;

	// Allows AI empires to colonize additional planets within systems they already control.
	bool enableIntraSystemColonization;

	// When colonizing within owned systems, determines whether T0 planets should be excluded.
	bool excludeT0PlanetColonization;

	eastl::vector<cEmpirePtr> empires;

	int empiresPerSubCycle;

	int lastCycleTime;
	int cycleStep;
	int systemsColonized;
	eastl::vector<cEmpirePtr>::iterator empireToExpand;
	ResourceKey redSpice;
	ResourceKey yellowSpice;
	ResourceKey blueSpice;
	ResourceKey greenSpice;
	ResourceKey pinkSpice;
	ResourceKey purpleSpice;


	int elapsedTime;
};