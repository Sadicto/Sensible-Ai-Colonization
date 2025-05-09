#include "stdafx.h"
#include "DebugAiColonization.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"
#include <chrono>
#include <cEmpireTerraformingManager.h>
using namespace SporeModUtils;
using namespace Simulator;
DebugAiColonization::DebugAiColonization()
{
}


DebugAiColonization::~DebugAiColonization()
{
}


void DebugAiColonization::ParseLine(const ArgScript::Line& line)
{
	auto args = line.GetArguments(1);
	int val = mpFormatParser->ParseInt(args[0]);
	switch (val) {
	case(0): {
		TerraformingUtils::TerraformToTerrascore(GetActivePlanetRecord(), PlanetType::T0);
		break;
	}
	case(1): {
		TerraformingUtils::TerraformToTerrascore(GetActivePlanetRecord(), PlanetType::T1);
		break;
	}
	case(2): {
		TerraformingUtils::TerraformToTerrascore(GetActivePlanetRecord(), PlanetType::T2);
		break;
	}
	case(3): {
		auto start = std::chrono::high_resolution_clock::now();
		TerraformingUtils::TerraformToTerrascore(GetActivePlanetRecord(), PlanetType::T3);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		int c = 5;
		break;
	}
	case(4): {
		cPlanetRecord* planet = GetActivePlanetRecord();
		int c = 4;
		break;
	}
	case(5): {
		cPlanetRecord* planet = GetActivePlanetRecord();
		planet->mCivData.clear();
		cPlanetRecord::FillPlanetDataForTechLevel(planet, TechLevel::Empire);
		cStarRecord* star = planet->GetStarRecord();
		cEmpire* empire = StarManager.GetEmpire(star->mEmpireID);
		for (auto civData : planet->mCivData) {
			civData->mPoliticalID = planet->GetStarRecord()->mEmpireID;
		}
		break;
		   }
	case(6): {
		eastl::map<ResourceKey, float> spicesCost;
		SpiceUtils::GetSpiceBaseCosts(spicesCost);
		int c = 5;
		break;
	}
	case(7): {
		eastl::map<ResourceKey, float> spicesCost;
		SpiceUtils::GetSpawnableSpiceBaseCosts(spicesCost);
		int c = 5;
		break;
	}
	case(8): {
		eastl::map<ResourceKey, float> spicesCost;
		SpiceUtils::GetSpawnableSpiceBaseCosts(spicesCost);
		ResourceKey minimunSpice = SpiceUtils::GetCheapestSpice(spicesCost);
		int d = 5;
		break;
	}
	case(9): {
		eastl::vector<cPlanetRecordPtr> planets;
		cEmpire* empire = StarManager.GetEmpire(GetActiveStarRecord()->mEmpireID);
		EmpireUtils::GetEmpirePlanets(empire, planets, false, false, false, true);
		int c = 5;
		break;
	}
	case(10): {
		cEmpireTerraformingManager* terraformingManager = cEmpireTerraformingManager::Get();
		bool terraformablePlanet = terraformingManager->TerraformablePlanet(GetActivePlanetRecord());
		int g = 5;
		break;
	}
	case(11): {
		cEmpireTerraformingManager* terraformingManager = cEmpireTerraformingManager::Get();
		cEmpire* empire = StarManager.GetEmpire(GetActiveStarRecord()->mEmpireID);
		bool canTerraform = terraformingManager->EmpireCanTerraformPlanet(empire, GetActivePlanetRecord());
		int b = 4;
		break;
	}
	case(12): {
		cEmpireTerraformingManager* terraformingManager = cEmpireTerraformingManager::Get();
		cEmpire* empire = StarManager.GetEmpire(GetActiveStarRecord()->mEmpireID);
		float p = terraformingManager->EmpireTerraformingProbability(empire);
		int b = 2;
		break;
	}
	case(13): {
		TerraformingUtils::TerraformingObstacle obstable = TerraformingUtils::GetTerraformingObstacle(GetActivePlanetRecord(), static_cast<PlanetType>(static_cast<int>(GetActivePlanetRecord()->mType) + 1));
		int b = 4;
		break;
	}
	case(14): {
		cEmpireTerraformingManager* terraformingManager = cEmpireTerraformingManager::Get();
		int value = terraformingManager->GetTerraformingValue(GetActivePlanetRecord());
		int b = 2;
		break;
	}
	case(15): {
		cEmpireTerraformingManager* terraformingManager = cEmpireTerraformingManager::Get();
		cEmpire* empire = StarManager.GetEmpire(GetActiveStarRecord()->mEmpireID);
		terraformingManager->EmpireTerraformPlanet(empire);
	}
	}
}

const char* DebugAiColonization::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "DebugAiColonization: Elaborate description of what this cheat does.";
	}
}
