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

	/**
	 * @brief Populates the cachedEcosystemCreatures vector and
	 * initializes the nextElementToReturn and nextElementToRefresh iterators.
	 * Sets cacheReady to true.
	 */
	void PrepareCache();

	/**
	 * @brief Replaces the ecosystem at nextElementToRefresh with a new one
	 * and advances the iterator.
	 */
	void RefreshElement();

	/**
	 * @brief Returns the ecosystem at nextElementToReturn and advances the iterator.
	 * @return Pointer to a vector of ResourceKeys representing a creature ecosystem.
	 */
	eastl::vector<ResourceKey>* ReturnElement();

	// Indicates whether the cache is ready to return elements.
	bool cacheReady;


private:
	
	static cEcosystemCreaturesCache* instance;

	// Interval between cycles (in milliseconds).
	int cycleInterval;

	// Number of cached entries to store.
	int cacheSize;

	// Cached creature ecosystems, each entry is a vector of ResourceKeys.
	eastl::vector<eastl::vector<ResourceKey>> cachedEcosystemCreatures;

	// Iterator pointing to the next cached entry to return.
	eastl::vector<eastl::vector<ResourceKey>>::iterator nextElementToReturn;

	// Iterator pointing to the next cached entry to refresh.
	eastl::vector<eastl::vector<ResourceKey>>::iterator nextElementToRefresh;

	// Time (in milliseconds) elapsed since the last refresh.
	int elapsedTime;

};