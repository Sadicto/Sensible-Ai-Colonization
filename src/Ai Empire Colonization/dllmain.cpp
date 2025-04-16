// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "cEmpireColonizationManager.h"


member_detour(FunctionHigh2Detour, cStarManager, void(cPlanetRecord*, cStarRecord*))
{
	void detoured(cPlanetRecord* planet, cStarRecord*  star)
	{

		original_function(this, planet, star);
	}
};

member_detour(FunctionHigh1Detour, cStarManager, void(cPlanetRecord*, cStarRecord*))
{
	void detoured(cPlanetRecord * planet, cStarRecord * star)
	{

		original_function(this, planet, star);
	}
};

member_detour(GeneratePlanetPlants, cStarManager, void(eastl::vector<ResourceKey>*, int, int, int, char, eastl::vector<ResourceKey>*))
{
	void detoured(eastl::vector<ResourceKey>*terrainScriptPlants, int terrascore, int terrascoreMinus1, int numOfPlantsInHighestScore, char unkModifier, eastl::vector<ResourceKey>* planetPlants)
	{
		//eastl::vector<ResourceKey> pointer = reinterpret_cast<eastl::vector<ResourceKey>>(&param2);
		//GameNounIDs noun = static_cast<GameNounIDs>(pointer->GetNounID());
		//ResourceKey* end = param7[1];
		//int capacity = end - param7[0];
		//int* valor = *reinterpret_cast<int**>(param2);
		//ResourceKey* key = reinterpret_cast<ResourceKey*>(param2);

		original_function(this, terrainScriptPlants, terrascore, terrascoreMinus1, numOfPlantsInHighestScore, unkModifier, planetPlants);
	}
};

member_detour(GetPlantsByTerrainScript, cStarManager, void(ResourceKey*, eastl::vector<ResourceKey>*))
{
	void detoured(ResourceKey* terrainScript, eastl::vector<ResourceKey>* plantsVector)
	{
		original_function(this, terrainScript, plantsVector);

	}
};

member_detour(GeneratePlanetCreatures, cStarManager, void(eastl::vector<ResourceKey>*, int , int , int , int , eastl::vector<ResourceKey>* ))
{
	void detoured(eastl::vector<ResourceKey>* priorityCreatures, int terrascore, int terrascoreMinusOne, int numOfCreaturesInHighestScore, int unkModifier, vector<ResourceKey>*planetCreatures)
	{
		original_function(this, priorityCreatures, terrascore,  terrascoreMinusOne, numOfCreaturesInHighestScore, unkModifier, planetCreatures);
	}
};

void Initialize()
{
	//ModAPI::AddSimulatorStrategy(system, cEmpireColonizationManager::NOUN_ID);
	cSimulatorSystem::Get()->AddStrategy(new cEmpireColonizationManager(), cEmpireColonizationManager::NOUN_ID);
}

void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));

	FunctionHigh2Detour::attach(Address(0x00BADEA0));
	FunctionHigh1Detour::attach(Address(0x00BAD5D0));
	GeneratePlanetPlants::attach(Address(0x00BAC9C0));
	GetPlantsByTerrainScript::attach(Address(0x00BAD210));
	GeneratePlanetCreatures::attach(Address(0x00BACE60));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

