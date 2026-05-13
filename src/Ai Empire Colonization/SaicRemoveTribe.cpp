#include "stdafx.h"
#include "SaicRemoveTribe.h"
#include "Spore-Mod-Utils/Include/SporeModUtils.h"

using namespace SporeModUtils;
using namespace Simulator;

SaicRemoveTribe::SaicRemoveTribe()
{
}


SaicRemoveTribe::~SaicRemoveTribe()
{
}


void SaicRemoveTribe::ParseLine(const ArgScript::Line& line) {
    if (!IsSpaceGame()) {
        App::ConsolePrintF("This command must be used in the space stage");
        return;
    }
    if (GetCurrentContext() != SpaceContext::SolarSystem) {
        App::ConsolePrintF("This command must be used in the solar system view");
        return;
    }
    cPlanetRecord* currentPlanet = GetActivePlanetRecord();
    if (!PlanetUtils::InteractablePlanet(currentPlanet)) {
        App::ConsolePrintF("This is not a valid planet");
        return;
    }

    if (currentPlanet->GetTechLevel() != TechLevel::Empire) {
        App::ConsolePrintF("No empire has colonized this planet");
        return;
    }

    if (!currentPlanet->mTribeData.empty()) {
        currentPlanet->mTribeData.clear();
        App::ConsolePrintF("Tribes removed, try entering the planet");
    }
    else {
        App::ConsolePrintF("No tribes to remove");
    }
}

const char* SaicRemoveTribe::GetDescription(ArgScript::DescriptionMode mode) const {
    if (mode == ArgScript::DescriptionMode::Basic) {
        return "Remove tribes from a planet";
    }
    else {
        return "SaicRemoveTribe: Remove tribes from a planet. Use this command from the star view while orbiting the planet whose tribes you want to remove.";
    }
}
