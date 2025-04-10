#include "stdafx.h"
#include "cEmpireExpansionManager.h"
#include <algorithm>

/// AUTOGENERATED METHODS ///

int cEmpireExpansionManager::AddRef() {
	return Simulator::cStrategy::AddRef();
}
int cEmpireExpansionManager::Release() {
	return Simulator::cStrategy::Release();
}

const char* cEmpireExpansionManager::GetName() const {
	return "AiEmpireExpansion::cEmpireExpansionManager";
}

bool cEmpireExpansionManager::Write(Simulator::ISerializerStream* stream)
{
	return Simulator::ClassSerializer(this, ATTRIBUTES).Write(stream);
}
bool cEmpireExpansionManager::Read(Simulator::ISerializerStream* stream)
{
	return Simulator::ClassSerializer(this, ATTRIBUTES).Read(stream);
}

bool cEmpireExpansionManager::WriteToXML(XmlSerializer*)
{
	return true;
}

/// END OF AUTOGENERATED METHODS ///
////////////////////////////////////

Simulator::Attribute cEmpireExpansionManager::ATTRIBUTES[] = {
	// Add more attributes here
	// This one must always be at the end
	Simulator::Attribute()
};

cEmpireExpansionManagerPtr cEmpireExpansionManager::instance = nullptr;

void cEmpireExpansionManager::Initialize() {
	instance = cEmpireExpansionManagerPtr(this);

	PropertyListPtr propList;

	PropManager.GetPropertyList(id("Config"), id("SaeeConfig"), propList);

	App::Property::GetFloat(propList.get(), 0x676FDB24, activeRadius);

	App::Property::GetInt32(propList.get(), 0x964CF55A, cycleInterval);

	App::Property::GetFloat(propList.get(), 0xDF83E9A3, apexCantSystems);

	App::Property::GetInt32(propList.get(), 0xD226209D, levelToColonizeTribe);

	App::Property::GetInt32(propList.get(), 0xBEE4774B, levelToColonizeCiv);

	App::Property::GetArrayFloat(propList.get(), 0xDFC93C59, colonizationRange);

	elapsedTime = 0;

	//idk if there�s a better way to do this lol.
	eastl::string16 stringRed(u"spice1");
	ResourceKey::Parse(redSpice, stringRed.c_str());

	eastl::string16 stringYellow(u"spice3");
	ResourceKey::Parse(yellowSpice, stringYellow.c_str());

	eastl::string16 stringBlue(u"spice2");
	ResourceKey::Parse(blueSpice, stringBlue.c_str());

	eastl::string16 stringGreen(u"spice4");
	ResourceKey::Parse(greenSpice, stringGreen.c_str());

	eastl::string16 stringPink(u"spice5");
	ResourceKey::Parse(pinkSpice, stringPink.c_str());

	eastl::string16 stringPurple(u"spice6");
	ResourceKey::Parse(purpleSpice, stringPurple.c_str());
	
	ResourceKey::Parse(adventureIconKey, u"0x0199b485!0x881aeb0a.0x2f7d0004");
}

void cEmpireExpansionManager::Dispose() {
	
}

void cEmpireExpansionManager::Update(int deltaTime, int deltaGameTime) {
	if (IsSpaceGame()) {
		elapsedTime += deltaGameTime;
		if (elapsedTime > cycleInterval) {
			EmpiresExpansionCycle();
			elapsedTime = 0;
		}
	}
}

void cEmpireExpansionManager::OnModeEntered(uint32_t previousModeID, uint32_t newModeID) {
	if (newModeID == GameModeIDs::kGameSpace) {
		elapsedTime = 0;

		cPlayer* player = GameNounManager.GetPlayer();
		Difficulty difficulty = player->mDifficultyLevel;

		PropertyListPtr propList;
		PropManager.GetPropertyList(id("Config"), id("SaeeConfig"), propList);

		eastl::vector<float> rMeanOneSystem;
		App::Property::GetArrayFloat(propList.get(), 0x5D39EA40, rMeanOneSystem);
		meanOneSystem = rMeanOneSystem[difficulty];

		eastl::vector<float> rMaxMean;
		App::Property::GetArrayFloat(propList.get(), 0xA1A4A55B, rMaxMean);
		maxMean = rMaxMean[difficulty];

		eastl::vector<float> rCyclesToApexColonies;
		App::Property::GetArrayFloat(propList.get(), 0x67F6019E, rCyclesToApexColonies);
		cyclesToApexColonies = rCyclesToApexColonies[difficulty];
	}
	cStrategy::OnModeEntered(previousModeID, newModeID); //idk if it is necessary.
}

cEmpireExpansionManagerPtr cEmpireExpansionManager::Get() {
	return instance;
}

cStarRecord* cEmpireExpansionManager::GetCurrentStar() {
	cStarRecord* actualStar = SpacePlayerData::Get()->mpActiveStar.get()->mpStarRecord.get();
	return actualStar;
}

cEmpire* cEmpireExpansionManager::GetOwnerOfCurrentSystem() {
	cStarRecord* actualStar = GetCurrentStar();
	cEmpire* empire = cStarManager::Get()->GetEmpireForStar(actualStar);
	return empire;
}

bool cEmpireExpansionManager::ColonizablePlanet(cPlanetRecord* planet) { //TODO 201planet.
	PlanetType type = planet->mType;
	return ((type == PlanetType::T3 ||
		type == PlanetType::T2 ||
		type == PlanetType::T1 ||
		type == PlanetType::T0) &&
		(cPlanetRecord::GetTypeIconKey(planet) != adventureIconKey && //not adventure, idk if there's a better way to do this
		!planet->IsDestroyed()));
}

int cEmpireExpansionManager::PlanetColonizationScore(cPlanetRecord* planet) {
    int score = 1;
	//if habitable, more terrascore more points.
    if (planet->mType == PlanetType::T3 || 
        planet->mType == PlanetType::T2 ||
        planet->mType == PlanetType::T1) {
        score = score + (static_cast<int>(planet->mType) * 10);
    }
	//if T0, green orbit more points.
    else { 
        if (((planet->mFlags & kPlanetFlagRedOrbit) == 0) &&
            ((planet->mFlags & kPlanetFlagBlueOrbit) == 0)) {
            score = score + 10;
        }
    }
	//moon.
    if (planet->IsMoon()) {
        score = score / 2;
    }

    ResourceKey spiceGen = planet->mSpiceGen;

    if (spiceGen == redSpice) {
        score++;
    }
    else if (spiceGen == yellowSpice) {
        score = score + 3;
    }
    else if (spiceGen == blueSpice) {
        score = score + 4;
    }
    else if (spiceGen == greenSpice) {
        score = score + 6;
    }
    else if (spiceGen == pinkSpice) {
        score = score + 8;
    }
    else if (spiceGen == purpleSpice) {
        score = score + 10;
    }
    return score;
}

bool cEmpireExpansionManager::ColonizableStar(cStarRecord* star) { 

	StarType type = star->GetType();
	//a star, unclaimed, no monolith not Sol and near.
	if (
		type != StarType::GalacticCore &&
		type != StarType::BlackHole &&
		type != StarType::ProtoPlanetary &&
		star->GetTechLevel() != TechLevel::Empire &&
		(star->mFlags & (1 << 3)) == 0 && //no monolith
		(star->mFlags & (1 << 1)) == 0 && //no savegame
		star != StarManager.GetSol() &&
		GetDistanceBetweenStars(GetCurrentStar(), star) < activeRadius) {

		//at least one planet is colonizable.
		for (cPlanetRecordPtr planet : star->GetPlanetRecords()) {
			if (ColonizablePlanet(planet.get())){
				return (true);
			}
		}
	}
	return false;
}

bool cEmpireExpansionManager::EmpireCanColonizeStar(cEmpire* empire, cStarRecord* star){
	TechLevel techLevel = star->GetTechLevel();
	int empireLevel = empire->field_D8;

	return ColonizableStar(star) 
		&& (techLevel == TechLevel::None || techLevel == TechLevel::Creature
		|| (techLevel == TechLevel::Tribe && empireLevel >= levelToColonizeTribe)
		|| (techLevel == TechLevel::Civilization && empireLevel >= levelToColonizeCiv));
}


cPlanetRecordPtr cEmpireExpansionManager::BestColonizablePlanet(cStarRecord* star) {
	//first filter all the non colonizable planets.
	eastl::vector<cPlanetRecordPtr> colonizablePlanets;

	for (cPlanetRecordPtr planet : star->GetPlanetRecords()) {
		if (ColonizablePlanet(planet.get())) {
			colonizablePlanets.push_back(planet); 
		}
	}

	cPlanetRecordPtr bestPlanet = colonizablePlanets.front();
	int bestScore = PlanetColonizationScore(bestPlanet.get());
	colonizablePlanets.erase(colonizablePlanets.begin());

	//choose the best planet.
	for (cPlanetRecordPtr planet : colonizablePlanets) {
		int planetScore = PlanetColonizationScore(planet.get());
		if (planetScore > bestScore) {
			bestPlanet = planet;
			bestScore = planetScore;
		}
	}
	return bestPlanet;
}

float cEmpireExpansionManager::StarColonizationScore(cStarRecord* star) {
	float score = PlanetColonizationScore(BestColonizablePlanet(star).get());

	if (star->GetTechLevel() == TechLevel::Tribe) {
		score = score / 5;
	}
	else if (star->GetTechLevel() == TechLevel::Civilization) {
		score = score / 20;
	}
	return score;
}

void  cEmpireExpansionManager::DeleteTribeFromStar(cStarRecord* star) {
	star->mTechLevel = TechLevel::Creature;
	star->mCitizenSpeciesKey.instanceID = 0;
	star->mCitizenSpeciesKey.typeID = 0;
	star->mCitizenSpeciesKey.groupID = 0;
	star->mpSpeciesProfile = NULL;

	//find the planet with the tribe and edit it .
	for (cPlanetRecordPtr planet : star->GetPlanetRecords()) {
		if (planet->GetTechLevel() == TechLevel::Tribe) {
			planet->mTechLevel = TechLevel::Creature;
			planet->mTribeData.clear();
		}
		break;
	}
}

void cEmpireExpansionManager::DeleteCivFromStar(cStarRecord* star) {
	star->mTechLevel = TechLevel::Creature;
	star->mCitizenSpeciesKey.instanceID = 0;
	star->mCitizenSpeciesKey.typeID = 0;
	star->mCitizenSpeciesKey.groupID = 0;
	star->mpSpeciesProfile = NULL;

	//find the planet with the civ and edit it. 
	for (cPlanetRecordPtr planet : star->GetPlanetRecords()) {
		if (planet->GetTechLevel() == TechLevel::Civilization) {
			planet->mTechLevel = TechLevel::Creature;
			planet->mCivData.clear();
		}
		break;
	}
}

SolarSystemOrbitTemperature cEmpireExpansionManager::GetPlanetOrbitTemperature(cPlanetRecord* planet){
	if (planet->mFlags & PlanetFlags::kPlanetFlagBlueOrbit) {
		return SolarSystemOrbitTemperature::Cold;
	}
	else if (planet->mFlags & PlanetFlags::kPlanetFlagRedOrbit){
		return SolarSystemOrbitTemperature::Hot;
	}
	else {
		return SolarSystemOrbitTemperature::Normal;
	}
}


void cEmpireExpansionManager::GeneratePlanets(cStarRecord* star) {
	cStar* starT = simulator_new<cStar>();
	starT->mpStarRecord = star;
	starT->GetSolarSystem();
}

void cEmpireExpansionManager::ColonizeStarSystem(cEmpire* empire, cStarRecord* star) {
	cPlanetRecordPtr planet = BestColonizablePlanet(star);
	uint32_t empireId = empire->GetEmpireID();

	empire->AddStarOwnership(star);
	star->mTechLevel = TechLevel::Empire;
	star->mEmpireID = empireId;

	planet->mTechLevel = TechLevel::Empire;
	cPlanetRecord::FillPlanetDataForTechLevel(planet.get(), TechLevel::Empire);
	for (auto civData : planet->mCivData) {
		civData->mPoliticalID = empireId;
	}
}

void cEmpireExpansionManager::GetEmpiresInRadius(const Vector3& coords, float radius, eastl::vector<cEmpirePtr>& empires) {
	StarRequestFilter filter;
	filter.RemoveStarType(StarType::None);
	filter.RemoveStarType(StarType::GalacticCore);
	filter.RemoveStarType(StarType::ProtoPlanetary);
	filter.RemoveStarType(StarType::BlackHole);
	filter.techLevels = 0;
	filter.AddTechLevel(TechLevel::Empire);
	filter.minDistance = 0;
	filter.maxDistance = radius;

	eastl::vector<cStarRecordPtr> starsColonized;
	StarManager.FindStars(coords, filter, starsColonized);

	// a set prevents duplicates.
	eastl::set<uint32_t> empireIDSet; 

	uint32_t playerEmpireId = SpacePlayerData::Get()->mPlayerEmpireID;
	uint32_t groxEmpireID = StarManager.GetGrobEmpireID();

	// Collect unique mEmpireID values owners if the stars, except the grox and player empire.
	for (cStarRecordPtr star : starsColonized) {
		if (star->mEmpireID != playerEmpireId && star->mEmpireID != groxEmpireID) {
			empireIDSet.insert(star->mEmpireID);
		}
	}
	//Get the empire for every id.
	for (uint32_t id : empireIDSet) {
		empires.push_back(cEmpirePtr(StarManager.GetEmpire(id)));
	}
}

void cEmpireExpansionManager::GetUnclaimedStarsInRadius(const Vector3& coords, float radius, eastl::vector<cStarRecordPtr>& stars) {
	StarRequestFilter filter;
	filter.RemoveStarType(StarType::None);
	filter.RemoveStarType(StarType::GalacticCore);
	filter.RemoveStarType(StarType::ProtoPlanetary);
	filter.RemoveStarType(StarType::BlackHole);
	filter.techLevels = 0;

	//even stars with all planet in T0 have this techLevel
	filter.AddTechLevel(TechLevel::Creature); 

	//we don't care about tribes or civilizations
	filter.AddTechLevel(TechLevel::Tribe);
	filter.AddTechLevel(TechLevel::Civilization);

	filter.minDistance = 0;
	filter.maxDistance = radius;
	cStarManager* starManager = cStarManager::Get();
	starManager->FindStars(coords, filter, stars);
}

float cEmpireExpansionManager::GetDistanceBetweenStars(cStarRecord* star1, cStarRecord* star2) {
	return (star1->mPosition - star2->mPosition).Length();
}

void cEmpireExpansionManager::ExpandEmpire(cEmpire* empire) {
	cStarRecord* homeworld = empire->GetHomeStarRecord();
	float range = colonizationRange[empire->field_D8];
	eastl::vector<cStarRecordPtr> empireStars = empire->mStars;
	cStarRecordPtr candidateStar = NULL;

	// Scores for each nearby colonizable star; the star with the highest score will be colonized.
	eastl::map<cStarRecordPtr, float> starsScores;

	// Find all colonizable stars within nearStarRadius from each empireStar and calculate their score.
	for (cStarRecordPtr empireStar : empireStars) {

		eastl::vector<cStarRecordPtr> nearUnclaimedStars;
		GetUnclaimedStarsInRadius(empireStar->mPosition, range, nearUnclaimedStars);
		for (cStarRecordPtr nearStar : nearUnclaimedStars) {
			// Planets are needed in each candidate star for the algorithm to function properly.
			StarManager.RequirePlanetsForStar(nearStar.get());

			// Check if the star is colonizable and the player is not currently in it.
			if (EmpireCanColonizeStar(empire, nearStar.get()) && nearStar.get() != GetCurrentStar()) {
				// If the star is not in the map, add it and calculate its base score.
				if (starsScores.count(nearStar) == 0) {
					// The further the star is from the homeworld, the lower the score.
					starsScores[nearStar] = StarColonizationScore(nearStar.get()) / (GetDistanceBetweenStars(homeworld, nearStar.get()) * 2);
				}
				else {
					// If the star is already in the map, it means it's close to multiple stars, so increase its score.
					starsScores[nearStar] *= 2;
				}
			}
		}
		float maxScore = -1;
		// find the Star with the highest Score.
		for (const auto& pair : starsScores) {
			if (pair.second > maxScore) {
				maxScore = pair.second;
				candidateStar = pair.first;
			}
		}
	}
	// If candidateStar is NULL, then there isn't a nearby colonizable star, so the empire does not expand.
	if (candidateStar != NULL) {
		if (candidateStar->GetTechLevel() == TechLevel::Tribe) {
			DeleteTribeFromStar(candidateStar.get());
		}
		else if (candidateStar->GetTechLevel() == TechLevel::Civilization) {
			DeleteCivFromStar(candidateStar.get());
		}
		GeneratePlanets(candidateStar.get());
		ColonizeStarSystem(empire, candidateStar.get());
	}
}

float cEmpireExpansionManager::EmpireExpansionProbability(cEmpire* empire) {

	// 1 + 2 + 3 + .... i, needed to calculate meanDiff.
	float total_sum_i = ((apexCantSystems - 1) * apexCantSystems) / 2;

	// The difference of mean between i and i + 1 systems.
	float meanDiff = std::abs((cyclesToApexColonies - meanOneSystem * (apexCantSystems - 1)) / total_sum_i);

	float cantSystems = empire->mStars.size();

	// the average wait given cantSystems
	float averageForCantSystems;

	// If the empire has more systems than the apex, use meanDiff to increase the average wait cycles.
	if (cantSystems > apexCantSystems) { 
		averageForCantSystems = meanOneSystem + (cantSystems + ((cantSystems - apexCantSystems) * 2) - apexCantSystems * 2) * meanDiff;
	}
	// If it has fewer systems, decrease the average.
	else {
		averageForCantSystems = meanOneSystem - (cantSystems - 1) * meanDiff; 
	}
	// In this distribution, p is 1/average; we use the maxMean to ensure the probability never gets too low.
	return std::max(1 / averageForCantSystems, 1 / maxMean); 
}

void cEmpireExpansionManager::EmpiresExpansionCycle() {
	// Only empires within activeRadius parsecs can expand.
	eastl::vector <cEmpirePtr> nearEmpires;

	GetEmpiresInRadius(GetCurrentStar()->mPosition, activeRadius, nearEmpires);
	// For each nearby empire, calculate pOfExpansion and expand it based on the probability.
	for (cEmpirePtr empire : nearEmpires) {
		float pOfExpansion = EmpireExpansionProbability(empire.get());
		float n = Math::randf();
		if (pOfExpansion > n) {
			ExpandEmpire(empire.get());
		}
	}
}




