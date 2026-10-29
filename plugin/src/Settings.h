/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#pragma once

// Fishing2026.dll settings, kept in Data/SKSE/Plugins/Fishing2026/settings.ini (written on first run, safe to edit while the game is closed).
namespace Settings
{
	struct Values
	{
		bool        blockHandFishing = true;  // swimming fish cannot be grabbed by hand: you need a fishing rod
		std::string handFishingMessage = "You need a fishing rod to catch fish.";
		std::string catchText = "Catch";                // the activation text a catchable thing shows; empty = do not look at the text
		std::string fishModelWords = "fish,fxambwater";  // a catchable thing is a fish when its model path has one of these words
	};

	Values& Get();
	void    Load();
	void    Save();
}
