#pragma once

#include <Spore\BasicIncludes.h>

#define cEmpireTerraformingManagerPtr intrusive_ptr<cEmpireTerraformingManager>

///
/// In your dllmain Initialize method, add the system like this:
/// ModAPI::AddSimulatorStrategy(new cEmpireTerraformingManager(), cEmpireTerraformingManager::NOUN_ID);
///

class cEmpireTerraformingManager
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("Ai_Empire_Expansion::cEmpireTerraformingManager");
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
	void OnModeEntered(uint32_t previousModeID, uint32_t newModeID) override;

	//
	// You can add more methods here
	//

	static Simulator::Attribute ATTRIBUTES[];

	static cEmpireTerraformingManager* Get();

	/**
	 * @brief Checks whether the current planet is terraformable.
	 * Takes into account the settings on the manager.
	 * @param planet
	 * @return True if the planet is terraformable, false otherwise.
	 */
	bool TerraformablePlanet(Simulator::cPlanetRecord* planet);

	/**
	 * @brief Checks whether the given empire can terraform the specified planet.
	 * Takes into account the terraforming obstacle, the empire level and if TerraformablePlanet(planet).
	 * @param empire
	 * @param planet
	 * @return True if the empire can terraform the planet, false otherwise.
	 */
	bool EmpireCanTerraformPlanet(Simulator::cEmpire* empire, Simulator::cPlanetRecord* planet);

	/**
	 * @brief Calculates the probability of an empire performing a terraforming action during this cycle.
	 * Preconditions: none.
	 * @param empire
	 * @return Float between 0 and 1 representing the probability of terraforming.
	 */
	float EmpireTerraformingProbability(Simulator::cEmpire* empire);

	/*
	 * Returns a custom value representing how desirable it is to terraform the given planet.
	 * Higher values indicate higher terraform priority.
	 *
	 * @param planet
	 * @return A numeric score representing the terraform potential of the planet.
	 */
	float GetTerraformingValue(Simulator::cPlanetRecord* planet);

	/**
	 * @brief Finds the best planet that the given empire can terraform.
	 * Prioritizes with good spice and in green orbits.
	 * Returns nullptr if no valid planets are found.
	 * @param empire
	 * @return Pointer to the selected planet (cPlanetRecord*), or nullptr if none available.
	 */
	Simulator::cPlanetRecord* GetBestTerraformablePlanetForEmpire(Simulator::cEmpire* empire);

	/**
	 * @brief Terraform the best available planet for the given empire.
	 * The selected planet is determined using GetBestTerraformablePlanetForEmpire().
	 * Does nothing if no terraformable planet is found.
	 * Preconditions: none.
	 * @param empire Pointer to the empire (cEmpire*) performing terraforming.
	 */
	void EmpireTerraformPlanet(Simulator::cEmpire* empire);

	/**
	 * @brief Executes one cycle of the terraforming manager,
	 * evaluating nearby empires using EmpireTerraformingProbability()
	 * and attempting to terraform a planet if the probability check passes.
	 * Preconditions: none.
	 */
	void EmpiresTerraformingCycle();




private:
	static cEmpireTerraformingManager* instance;

	eastl::map<ResourceKey, float> spiceCosts;

	float activeRadius;

	bool galacticRadius;

	int cycleInterval;

	int elapsedTime;

	float terraformingPerHour;

	bool terraformAllowed;

	bool goodSpiceTerraformAllowed;

	bool badOrbitTerraformAllowed;

	int levelToDecreaseAtmosphere;

	int levelToIncreaseAtmosphere;

	int levelToDecreaseTemperature;

	int levelToIncreaseTemperature;


	//
	// You can add members here
	//
};