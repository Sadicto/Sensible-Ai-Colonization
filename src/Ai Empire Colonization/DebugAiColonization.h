#pragma once

#include <Spore\BasicIncludes.h>

class DebugAiColonization 
	: public ArgScript::ICommand
{
public:
	DebugAiColonization();
	~DebugAiColonization();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;

	Simulator::cEmpire* empire;
	Simulator::cStarRecord* star;
};

