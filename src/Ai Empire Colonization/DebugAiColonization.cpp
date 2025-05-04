#include "stdafx.h"
#include "DebugAiColonization.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"
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
		TerraformingUtils::TerraformToTerrascore(GetActivePlanetRecord(), PlanetType::T3);
		break;
	}
	case(4): {
		cPlanetRecord* planet = GetActivePlanetRecord();
		int c = 4;
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
