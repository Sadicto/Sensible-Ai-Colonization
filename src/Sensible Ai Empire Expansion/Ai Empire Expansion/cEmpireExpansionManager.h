#pragma once

#include <Spore\BasicIncludes.h>
#include <Spore/Simulator/Serialization.h>

#define cEmpireExpansionManagerPtr intrusive_ptr<cEmpireExpansionManager>

using namespace Simulator;

class cEmpireExpansionManager
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("AiEmpireExpansion::cEmpireExpansionManager");
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

	static cEmpireExpansionManagerPtr Get();

	/**
	 * @brief Determines if a given planet is colonizable, meaning it is a terrestrial planet,
	 * is not destroyed, and is not an adventure. (TODO check for 201 ruins planet)
	 * Preconditions: none.
	 * @param planet Pointer to the planet (cPlanetRecord*) to check for colonization.
	 * @return true if the planet is colonizable, false otherwise.
	 */
	bool ColonizablePlanet(cPlanetRecord* planet);

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
	 * @brief Calculates the colonization score of a star system
	 * using its highest planet score, reducing it if the star
	 * has tribes and further if it has a civilization.
	 * Preconditions: At least one planet in the star, ColonizablePlanet(planet).
	 * @param star Pointer to the star system (cStarRecord*) to evaluate.
	 * @return Float score representing the colonization potential of the star.
	 */
	float StarColonizationScore(cStarRecord* star);

	/**
	 * @brief Deletes a tribal species from a star system.
	 * Preconditions: none.
	 * @param star Pointer to the star system (cStarRecord*) containing the tribal civilization.
	 */
	void DeleteTribeFromStar(cStarRecord* star);

	/**
	 * @brief Deletes a civilization from a star system.
	 * Preconditions: none.
	 * @param star Pointer to the star system (cStarRecord*) containing the civilization.
	 */
	void DeleteCivFromStar(cStarRecord* star);

	/**
	* @brief Returns the orbit's type of a planet (hot, cold or normal).
	* Preconditions: none.
	* @param planet Pointer to the planet.
	*/
	SolarSystemOrbitTemperature GetPlanetOrbitTemperature(cPlanetRecord* planet);

	//
	/*
	*  @brief Assign spices to all planets in the stars and generates its orbits.
	* Preconditions: none.
	* @param star The star system in which the planets will be generated
	*/
	void GeneratePlanets(cStarRecord* star);

	/**
	 * @brief Colonizes a star system with the given empire.
	 * Preconditions: ColonizableStar(star) and no tribes or civilizations present.
	 * @param empire Pointer to the empire (cEmpire*) performing the colonization.
	 * @param star Pointer to the star system (cStarRecord*) being colonized.
	 */
	void ColonizeStarSystem(cEmpire* empire, cStarRecord* star);

	/**
	 * @brief Retrieves all empires within a given radius of coordinates,
	 * except for the Grox and the player empire.
	 * Preconditions: none.
	 * @param coords Vector3 representing the center coordinates.
	 * @param radius Float representing the search radius in parsecs.
	 * @param empires Vector to store the list of empires (eastl::vector<cEmpirePtr>&)
	 * found within the radius.
	 */
	void GetEmpiresInRadius(const Vector3& coords, float radius, eastl::vector<cEmpirePtr>& empires);


	/**
	 * @brief Retrieves all unclaimed stars (by an empire) within a given radius of coordinates.
	 * Preconditions: none.
	 * @param coords Vector3 representing the center coordinates.
	 * @param radius Float representing the search radius in parsecs.
	 * @param stars Vector to store the list of unclaimed stars (eastl::vector<cStarRecordPtr>&) found within the radius.
	 */
	void GetUnclaimedStarsInRadius(const Vector3& coords, float radius, eastl::vector<cStarRecordPtr>& stars);

	/**
	 * @brief Calculates the distance between two star systems.
	 * Preconditions: none.
	 * @param star1 Pointer to the first star system (cStarRecord*).
	 * @param star2 Pointer to the second star system (cStarRecord*).
	 * @return Float representing the distance between the two stars in parsecs.
	 */
	float GetDistanceBetweenStars(cStarRecord* star1, cStarRecord* star2);

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
	 * @brief Calculates the probability of an empire expanding to new systems
	 * based on its current number of stars.
	 * Preconditions: none.
	 * @param empire Pointer to the empire (cEmpire*) being evaluated.
	 * @return Float between 0 and 1 representing the probability of expansion.
	 */
	float EmpireExpansionProbability(cEmpire* empire);

	/**
	 * @brief Executes one cycle of the manager, expanding nearby empires
	 * according to EmpireExpansionProbability(empire).
	 * Preconditions: none.
	 */
	void EmpiresExpansionCycle();
	

private:

	
	static cEmpireExpansionManagerPtr instance;

	// Radius (in parsecs) in which empires colonize systems
	float activeRadius;

	// Miliseconds of gameTime between expansion cycles
	int cycleInterval;

	// Number of systems that maximize the probability of expansion in a cycle.
	float apexCantSystems;

	// Represents the average cycles necessary for an empire to colonize its first system.
	float avgOneSystem;

	// The maximum average cycles necessary to colonize the next system, regardless of the number of colonies.
	float maxAvg;

	// Average cycles necessary for an empire to reach apexCantSystems colonies.
	float cyclesToApexColonies;

	// Colonization range per empire level, in parsecs.
	eastl::vector<float> colonizationRange;

	// Minimun level for an empire to be able to colonize a star with a tribe.
	int levelToColonizeTribe;

	// Minimun level for an empire to be able to colonize a star with a civ.
	int levelToColonizeCiv;

	ResourceKey redSpice;
	ResourceKey yellowSpice;
	ResourceKey blueSpice;
	ResourceKey greenSpice;
	ResourceKey pinkSpice;
	ResourceKey purpleSpice;

	ResourceKey adventureIconKey;

	int elapsedTime;
};