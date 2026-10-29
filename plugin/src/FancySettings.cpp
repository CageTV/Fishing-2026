/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "FancySettings.h"

namespace Fancy
{
	namespace
	{
		constexpr const char* kFile = "Data/SKSE/Plugins/Fishing2026/fancy.ini";
		constexpr const char* kPlugin = "FancyFishing.esp";
		using K = Kind;
		using P = Page;

		// Labels, ranges and tips are Fancy Fishing's own (its MCM page); defaults are the ones in FancyFishing.esp, except the minigame, which is on.
		const Entry kTable[] = {
			// ---- Fishing
			{ P::Fishing, "Convenience", "AutoReel", 0x811, K::Toggle, 0, 1, 0, "Auto-reel", "Automatically reel fish in when hooked, instead of pressing the activate key." },
			{ P::Fishing, "Convenience", "RodAllowCheckBypass", 0x812, K::Toggle, 0, 1, 0, "Bypass rod check", "Always allow catching fish that would normally need a specific rod." },
			{ P::Fishing, "Convenience", "BiteTimeMult", 0x82F, K::Float, 0.2f, 5.0f, 1.0f, "Bite time", "Multiplier on the time fish take to nibble and bite. Things may look strange when this is very low." },
			{ P::Fishing, "Convenience", "DoFade", 0x833, K::Toggle, 0, 1, 1, "Fade", "Fade the screen to black when fishing starts." },
			{ P::Fishing, "Convenience", "AllowCameraMovement", 0x835, K::Toggle, 0, 1, 0, "Allow looking", "Let the camera move so you can look around while waiting for a bite." },
			{ P::Fishing, "Convenience", "ContinueFishing", 0x837, K::Toggle, 0, 1, 0, "Automatic re-cast", "After a catch or a lost fish the line is cast again by itself. Cancel before a fish is hooked to stop." },
			{ P::Fishing, "Fishing spots", "PopReduceChance", 0x820, K::Float, 0, 1, 1, "Depletion chance (success)", "Chance that a spot's fish population drops when you catch a fish. 1 = always." },
			{ P::Fishing, "Fishing spots", "PopReduceChanceFailed", 0x832, K::Float, 0, 1, 1, "Depletion chance (failure)", "Chance that a spot's fish population drops when a fish gets away after a nibble. 1 = always." },
			{ P::Fishing, "Rod display", "AllowWeapEquip", 0xA46, K::Toggle, 0, 1, 0, "Rod drawn", "Draw the rod, or leave it drawn, while fishing. Mostly for VR." },

			// ---- Odds
			{ P::Odds, "Fish", "ChanceRareBase", 0x824, K::Float, 0, 1, 0.03f, "Rare fish chance", "Base chance of rare fish, out of 1 for rare, uncommon and common together. Do not set rare + uncommon to 1 or more." },
			{ P::Odds, "Fish", "ChanceUncommonBase", 0x826, K::Float, 0, 1, 0.22f, "Uncommon fish chance", "Base chance of uncommon fish." },
			{ P::Odds, "Junk", "ChanceJunkBase", 0x821, K::Float, 0, 1, 0.10f, "Junk chance", "Base chance of hooking junk instead of a fish. Rises as a spot is depleted and with the Dwarven rod." },
			{ P::Odds, "Junk", "ChanceJunkMax", 0x823, K::Float, 0, 1, 1.0f, "Junk chance (max)", "Upper limit on the junk chance even at a depleted spot. 1 = no limit." },
			{ P::Odds, "Junk", "ChanceJunkRareBase", 0x822, K::Float, 0, 1, 0.03f, "Rare junk chance", "Base chance of rare junk, out of 1 for rare, uncommon and common junk together." },
			{ P::Odds, "Junk", "ChanceJunkUncommonBase", 0x825, K::Float, 0, 1, 0.32f, "Uncommon junk chance", "Base chance of uncommon junk." },
			{ P::Odds, "Perfect catch", "PerfectExtraJunkChance", 0x809, K::Float, 0, 1, 0.0f, "Bonus junk chance", "Chance of an extra item when the bar stays on the fish for the whole minigame." },
			{ P::Odds, "Perfect catch", "PerfectExtraJunkUseAltList", 0x829, K::Toggle, 0, 1, 0, "Use special list", "Draw bonus items from a special list (ingredients, gems, small jewellery, soul gems) instead of the usual junk lists." },

			// ---- Minigame
			{ P::Minigame, "Minigame", "MinigameType", 0x800, K::Toggle, 0, 1, 1, "Minigame", "The bar-and-fish minigame when a fish bites. Off = the usual catch prompt." },
			{ P::Minigame, "Look", "GlobalScale", 0x81A, K::Float, 0.1f, 1.0f, 0.67f, "Scale", "Size of the minigame; higher is larger." },
			{ P::Minigame, "Look", "BgdAlpha", 0x819, K::Float, 0, 1, 0.5f, "Background opacity", "How much the screen behind the minigame is darkened." },
			{ P::Minigame, "Look", "BarAlphaOff", 0x834, K::Float, 0, 1, 0.4f, "Bar opacity (escaping)", "How opaque the catch bar is while the fish is escaping." },
			{ P::Minigame, "Look", "CenterX", 0x83D, K::Int, -640, 640, 0, "Center (x)", "Horizontal position of the minigame, 0 = centre of the screen." },
			{ P::Minigame, "Look", "CenterY", 0x83E, K::Int, -360, 360, 0, "Center (y)", "Vertical position of the minigame, 0 = centre of the screen." },
			{ P::Minigame, "Look", "ShowPerfect", 0x943, K::Toggle, 0, 1, 1, "Perfect pop-up", "Show \"Perfect!\" after a perfect catch." },
			{ P::Minigame, "Difficulty", "ScaleBarMin", 0x80A, K::Float, 0.05f, 0.5f, 0.2f, "Bar size (min)", "Starting size of the catch bar as a fraction of the whole range." },
			{ P::Minigame, "Difficulty", "ScaleBarMax", 0x80E, K::Float, 0.05f, 0.5f, 0.2f, "Bar size (max)", "If different from the minimum, the bar grows to this as you catch more kinds of fish." },
			{ P::Minigame, "Difficulty", "ScaleSpeed", 0x80F, K::Float, 0.2f, 5.0f, 1.0f, "Fish speed", "How fast the fish moves." },
			{ P::Minigame, "Difficulty", "ScaleSpeedNoRod", 0x813, K::Float, 1.0f, 5.0f, 1.5f, "Fish speed (no rod)", "Extra fish speed when you lack the rod a fish usually needs (needs Bypass rod check)." },
			{ P::Minigame, "Difficulty", "SpeedMultBar", 0x81C, K::Float, 0.2f, 5.0f, 1.0f, "Bar speed", "How fast the catch bar moves." },
			{ P::Minigame, "Difficulty", "SpeedMultMeterUp", 0x81E, K::Float, 0.2f, 5.0f, 1.0f, "Catch speed", "How fast the fish is caught while you reel it in." },
			{ P::Minigame, "Difficulty", "SpeedMultMeterDown", 0x81D, K::Float, 0.2f, 5.0f, 1.0f, "Escape speed", "How fast the fish escapes while you do not reel." },
			{ P::Minigame, "Difficulty", "Momentum", 0x81B, K::Toggle, 0, 1, 1, "Momentum", "The bar moves with momentum and gravity. Off = it moves at a fixed speed." },
			{ P::Minigame, "Compatibility", "MinigameAsHudMenu", 0x80D, K::Toggle, 0, 1, 0, "Unpaused game", "Let the game run behind the minigame. More likely to cause trouble in a heavily modded game." },
			{ P::Minigame, "Compatibility", "MappableKeyControls", 0x838, K::Toggle, 0, 1, 0, "Custom controls", "Use the keys set on Fancy Fishing's MCM page (key re-mapping is still done there)." },
		};
		constexpr std::size_t kCount = sizeof(kTable) / sizeof(kTable[0]);

		RE::TESGlobal* globals[kCount] = {};
		float          saved[kCount] = {};
		bool           found = false;
		bool           applyOnLoad = true;

		void WriteIni()
		{
			std::error_code ec;
			std::filesystem::create_directories(std::filesystem::path(kFile).parent_path(), ec);
			std::ofstream out(kFile, std::ios::trunc);
			if (!out) {
				SKSE::log::warn("Cannot write {}", kFile);
				return;
			}
			out << "; Fancy Fishing's settings, kept here by the Fishing 2026 page of SKSE Menu Framework. They are written into the game at the start of every game and\n"
				<< "; after every load. Safe to edit while the game is closed.\n"
				<< "apply_on_load=" << (applyOnLoad ? 1 : 0) << "\n";
			for (std::size_t i = 0; i < kCount; ++i) {
				out << kTable[i].id << "=" << saved[i] << "\n";
			}
		}
	}

	const Entry* Table(std::size_t& a_count)
	{
		a_count = kCount;
		return kTable;
	}

	bool Available() { return found; }

	bool& ApplyOnLoad() { return applyOnLoad; }

	float Get(const Entry& a_e)
	{
		const auto i = static_cast<std::size_t>(&a_e - kTable);
		return (i < kCount && globals[i]) ? globals[i]->value : a_e.def;
	}

	void Set(const Entry& a_e, float a_value)
	{
		const auto i = static_cast<std::size_t>(&a_e - kTable);
		if (i >= kCount) {
			return;
		}
		a_value = std::clamp(a_value, a_e.min, a_e.max);
		if (a_e.kind != Kind::Float) {
			a_value = std::round(a_value);
		}
		saved[i] = a_value;
		if (globals[i]) {
			globals[i]->value = a_value;
		}
		WriteIni();
	}

	void ResetPage(Page a_page)
	{
		for (std::size_t i = 0; i < kCount; ++i) {
			if (kTable[i].page == a_page) {
				Set(kTable[i], kTable[i].def);
			}
		}
	}

	void Init()
	{
		for (std::size_t i = 0; i < kCount; ++i) {
			saved[i] = kTable[i].def;
		}
		auto* dh = RE::TESDataHandler::GetSingleton();
		if (!dh || !dh->LookupModByName(kPlugin)) {
			SKSE::log::info("Fancy Fishing settings: {} is not in the load order", kPlugin);
			return;
		}
		int ok = 0;
		for (std::size_t i = 0; i < kCount; ++i) {
			globals[i] = dh->LookupForm<RE::TESGlobal>(kTable[i].local, kPlugin);
			ok += globals[i] ? 1 : 0;
		}
		found = ok > 0;
		std::ifstream in(kFile);
		if (!in) {
			WriteIni();  // first run: the defaults, so the file exists and the minigame is on
		} else {
			std::string line;
			while (std::getline(in, line)) {
				const auto eq = line.find('=');
				if (eq == std::string::npos || line[0] == ';') {
					continue;
				}
				const auto key = line.substr(0, eq);
				const auto val = line.substr(eq + 1);
				try {
					if (key == "apply_on_load") {
						applyOnLoad = std::stof(val) != 0.0f;
						continue;
					}
					for (std::size_t i = 0; i < kCount; ++i) {
						if (key == kTable[i].id) {
							saved[i] = std::clamp(std::stof(val), kTable[i].min, kTable[i].max);
						}
					}
				} catch (...) {
				}
			}
		}
		SKSE::log::info("Fancy Fishing settings: {} of {} globals found, remembered values {}", ok, kCount, applyOnLoad ? "applied on every load" : "not applied");
	}

	void ApplyAll()
	{
		if (!found || !applyOnLoad) {
			return;
		}
		for (std::size_t i = 0; i < kCount; ++i) {
			if (globals[i]) {
				globals[i]->value = saved[i];
			}
		}
		SKSE::log::info("Fancy Fishing settings: remembered values written into the game");
	}
}
