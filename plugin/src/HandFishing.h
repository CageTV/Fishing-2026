/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#pragma once

// Swimming fish (the ambient salmon flora of the base game) cannot be grabbed by hand any more: the player has to fish for them.
namespace HandFishing
{
	void Install();       // hooks TESFlora::Activate
	void OnDataLoaded();  // finds the fish flora by their editor ids and writes what it found to the log
}
