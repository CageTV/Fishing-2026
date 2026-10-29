/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "Settings.h"

namespace Settings
{
	namespace
	{
		constexpr const char* kFile = "Data/SKSE/Plugins/Fishing2026/settings.ini";
		Values values;

		std::string Trim(const std::string& a_s)
		{
			const auto b = a_s.find_first_not_of(" \t\r\n");
			const auto e = a_s.find_last_not_of(" \t\r\n");
			return b == std::string::npos ? std::string() : a_s.substr(b, e - b + 1);
		}
	}

	Values& Get() { return values; }

	void Load()
	{
		std::ifstream in(kFile);
		if (!in) {
			Save();  // first run: write the defaults so the file exists
			return;
		}
		std::string line;
		while (std::getline(in, line)) {
			if (line.empty() || line[0] == ';' || line[0] == '#' || line[0] == '[') {
				continue;
			}
			const auto eq = line.find('=');
			if (eq == std::string::npos) {
				continue;
			}
			const std::string key = Trim(line.substr(0, eq));
			const std::string val = Trim(line.substr(eq + 1));
			if (key == "block_hand_fishing") {
				values.blockHandFishing = val == "1" || val == "true";
			} else if (key == "hand_fishing_message") {
				values.handFishingMessage = val;
			} else if (key == "catch_text") {
				values.catchText = val;
			} else if (key == "fish_model_words") {
				values.fishModelWords = val;
			}
		}
	}

	void Save()
	{
		std::error_code ec;
		std::filesystem::create_directories(std::filesystem::path(kFile).parent_path(), ec);
		std::ofstream out(kFile, std::ios::trunc);
		if (!out) {
			SKSE::log::warn("Cannot write {}", kFile);
			return;
		}
		out << "; Fishing 2026. Safe to edit by hand while the game is closed.\n"
			<< "; 1 = swimming fish (the salmon in streams) cannot be grabbed by hand, you need a fishing rod. Empty message = no message.\n"
			<< "block_hand_fishing=" << (values.blockHandFishing ? 1 : 0) << "\n"
			<< "hand_fishing_message=" << values.handFishingMessage << "\n";
	}
}
