/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#pragma once

// A stand-in for Fancy Fishing's minigame drawn on screen while its Minigame settings page is open, so its position, size, background and bar can be judged
// without catching a fish. It uses the sizes of the real Flash parts (frame 128 x 600, catch bar 46 x 600 * bar size, fish 64 x 64 on the 1280 x 720 stage)
// and is a plain outline: the real widget keeps its own look.
namespace Preview
{
	void Request();  // call every frame the page is drawn
	void Draw();     // call every frame from the HUD callback: draws only shortly after a Request
	bool& DimScreen();
}
