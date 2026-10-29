/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#pragma once

// Fancy Fishing keeps its settings in game globals (FancyFishing.esp) and shows them on a SkyUI MCM page. This is the same set, shown on an SKSE Menu
// Framework page and remembered in Data/SKSE/Plugins/Fishing2026/fancy.ini, so they hold for every character and new game. The minigame itself (the Flash
// widget in Interface/exported/widgets/fancyfishing) is untouched, so it can be reskinned like any widget.
namespace Fancy
{
	enum class Kind
	{
		Toggle,
		Float,
		Int
	};

	enum class Page
	{
		Fishing,
		Odds,
		Minigame
	};

	struct Entry
	{
		Page        page;
		const char* group;
		const char* id;  // the global's editor id without "__FancyFishingVar"; also the key in fancy.ini
		unsigned    local;  // form id inside FancyFishing.esp
		Kind        kind;
		float       min, max, def;
		const char* label;
		const char* tip;
	};

	const Entry*       Table(std::size_t& a_count);
	bool               Available();              // FancyFishing.esp is in the load order and its globals were found
	float              Get(const Entry& a_e);    // the value in the game now
	void               Set(const Entry& a_e, float a_value);  // game and fancy.ini
	void               ResetPage(Page a_page);
	void               Init();                   // after the game data is loaded: finds the globals, reads fancy.ini (written with defaults on the first run)
	void               ApplyAll();               // writes the remembered values into the game, at the start of a game and after a load
	bool&              ApplyOnLoad();
}
