#include "stdafx.h"
#include "DebugAiColonization.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"
#include <chrono>
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
		TerraformingUtils::TerraformingObstacle obstacle = TerraformingUtils::GetTerraformingObstacle(GetActivePlanetRecord(), PlanetType::T3);
		int c = 3;
		break;
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
