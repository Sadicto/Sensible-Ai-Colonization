#pragma once

#include <Spore\BasicIncludes.h>

/// Cheat that removes all tribes from a star. 
/// Used to fix a crash that occurs when an empire colonizes a tribe planet 
/// and the tribes aren't successfully deleted.
class SaicRemoveTribe 
	: public ArgScript::ICommand
{
public:
	SaicRemoveTribe();
	~SaicRemoveTribe();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;
};

