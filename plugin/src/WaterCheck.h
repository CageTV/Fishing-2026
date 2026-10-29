/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#pragma once

// Is there water to fish in, in front of the player? Replaces Fish Anywhere's test (an invisible projectile fired at the water, a hidden NPC moved to where
// it landed and asked whether it is "in water"), which failed at shallow shores. This looks at the water surface and the ground below it along the way
// the player faces.
namespace WaterCheck
{
	void RegisterPapyrus();  // Fishing2026Native.IsWaterAhead(Actor, float maxDistance, float minDepth) global native
}
