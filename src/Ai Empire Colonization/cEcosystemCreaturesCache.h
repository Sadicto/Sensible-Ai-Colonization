#pragma once

#include <Spore\BasicIncludes.h>

#define cEcosystemCreaturesCachePtr intrusive_ptr<cEcosystemCreaturesCache>

///
/// In your dllmain Initialize method, add the system like this:
/// ModAPI::AddSimulatorStrategy(new cEcosystemCreaturesCache(), cEcosystemCreaturesCache::NOUN_ID);
///

class cEcosystemCreaturesCache
	: public Simulator::cStrategy
{
public:
	static const uint32_t TYPE = id("Ai_Empire_Expansion::cEcosystemCreaturesCache");
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
	void OnModeExited(uint32_t previousModeID, uint32_t newModeID) override;

	//
	// You can add more methods here
	//

	static Simulator::Attribute ATTRIBUTES[];

	static cEcosystemCreaturesCache* Get();

	void PrepareCache();

	void RefreshElement();

	eastl::vector<ResourceKey>* ReturnElement();

	bool cacheReady;

private:
	
	static cEcosystemCreaturesCache* instance;

	int cycleInterval;

	int cacheSize;

	eastl::vector<eastl::vector<ResourceKey>> cachedEcosystemCreatures;

	eastl::vector<eastl::vector<ResourceKey>>::iterator nextElementToReturn;

	eastl::vector<eastl::vector<ResourceKey>>::iterator nextElementToRefresh;


	int elapsedTime;
};