// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "cEmpireColonizationManager.h"
#include "cEmpireTerraformingManager.h"
#include "cEcosystemCreaturesCache.h"
#include "DebugAiColonization.h"


void Initialize()
{
	bool empireColonizationManager = cSimulatorSystem::Get()->AddStrategy(new cEmpireColonizationManager(), cEmpireColonizationManager::NOUN_ID);
	bool empireTerraformingManager = cSimulatorSystem::Get()->AddStrategy(new cEmpireTerraformingManager(), cEmpireTerraformingManager::NOUN_ID);
	bool ecosystemCreaturesCache = cSimulatorSystem::Get()->AddStrategy(new cEcosystemCreaturesCache(), cEcosystemCreaturesCache::NOUN_ID);
	//CheatManager.AddCheat("DebugAiColonization", new DebugAiColonization());
}

void Dispose()
{
	// This method is called when the game is closing
}

member_detour(GetCreatures__detour, Simulator::cStarManager, void(eastl::vector<ResourceKey>*, int, int, int, int, eastl::vector<ResourceKey>*)) {
	void detoured(eastl::vector<ResourceKey>*a, int b, int c, int d, int e, eastl::vector<ResourceKey>*f) {
		cEcosystemCreaturesCache* creaturesCache = cEcosystemCreaturesCache::Get();
		if (IsSpaceGame() && creaturesCache->cacheReady) {
			eastl::vector<ResourceKey>* cachedCreatures = creaturesCache->ReturnElement();
			original_function(this, cachedCreatures, b, c, d, e, f);
		}
		else {
			original_function(this, a, b, c, d, e, f);
		}
	}
};

void AttachDetours()
{
	GetCreatures__detour::attach(Address(ModAPI::ChooseAddress(0x00BABC40, 0x00BACE60)));
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

