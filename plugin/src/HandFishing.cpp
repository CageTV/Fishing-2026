/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "HandFishing.h"
#include "Settings.h"

namespace HandFishing
{
	namespace
	{
		std::string Lower(std::string_view a_text)
		{
			std::string s(a_text);
			std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			return s;
		}

		std::vector<std::string> Split(const std::string& a_list)
		{
			std::vector<std::string> out;
			std::string              cur;
			for (char c : a_list) {
				if (c == ',' || c == ';') {
					if (!cur.empty()) {
						out.push_back(Lower(cur));
					}
					cur.clear();
				} else if (c != ' ') {
					cur += c;
				}
			}
			if (!cur.empty()) {
				out.push_back(Lower(cur));
			}
			return out;
		}

		int logged = 0;  // each distinct decision is written to the log a few times so a missed or wrongly caught thing can be found

		// What the base object of the thing the player is looking at says about itself: its activation text ("Catch") and its model.
		// Fish are the things that say "Catch" and are shown with a fish model: the base game's ambient salmon (FXAmbWater models), the pond fish critters
		// (critters\fish\...), the Creation Club bucket and tripod fish, Improved Fish's fish. Fish plaques, tanks, dead fish on racks, butterflies, crabs
		// and everything else are left alone: they do not say "Catch" with a fish model.
		bool IsFish(RE::TESObjectREFR* a_target, std::string& a_why)
		{
			auto* base = a_target ? a_target->GetBaseObject() : nullptr;
			if (!base) {
				return false;
			}
			const auto type = base->GetFormType();
			if (type != RE::FormType::Flora && type != RE::FormType::Activator && type != RE::FormType::Container) {
				return false;
			}
			const auto& cfg = Settings::Get();

			std::string text;
			if (auto* player = RE::PlayerCharacter::GetSingleton()) {
				RE::BSString s;
				if (base->GetActivateText(player, s) && s.c_str()) {
					text = s.c_str();
				}
			}
			const std::string wantText = Lower(cfg.catchText);
			if (!wantText.empty() && Lower(text).find(wantText) == std::string::npos) {
				return false;
			}

			std::string model;
			if (auto* tm = skyrim_cast<RE::TESModel*>(base)) {
				if (const char* m = tm->GetModel()) {
					model = Lower(m);
				}
			}
			for (const auto& w : Split(cfg.fishModelWords)) {
				if (model.find(w) != std::string::npos) {
					a_why = std::format("'{}' says \"{}\", model {} has \"{}\"", base->GetName(), text, model, w);
					return true;
				}
			}
			if (logged < 40) {
				++logged;
				SKSE::log::info("Hand fishing: left alone: '{}' ({:08X}) says \"{}\", model {} has no fish word", base->GetName(), base->GetFormID(), text, model);
			}
			return false;
		}

		struct Hook
		{
			static bool Thunk(RE::TESObjectREFR* a_target, RE::TESObjectREFR* a_activator, std::uint8_t a_arg2, RE::TESBoundObject* a_object, std::int32_t a_count,
				bool a_defaultOnly)
			{
				if (Settings::Get().blockHandFishing && a_activator && a_activator->IsPlayerRef()) {
					std::string why;
					if (IsFish(a_target, why)) {
						if (logged < 80) {
							++logged;
							SKSE::log::info("Hand fishing: blocked: {}", why);
						}
						const auto& msg = Settings::Get().handFishingMessage;
						if (!msg.empty()) {
							Notify(msg.c_str());
						}
						return true;  // handled: the fish swims on
					}
				}
				return func(a_target, a_activator, a_arg2, a_object, a_count, a_defaultOnly);
			}
			static inline REL::Relocation<decltype(Thunk)> func;
		};
	}

	void Install()
	{
		if (REL::Module::IsVR()) {
			SKSE::log::info("Hand fishing: Skyrim VR is not supported by this build, fish can still be grabbed by hand");
			return;
		}
		// The player's "press the activate key" function calls TESObjectREFR::ActivateRef exactly once: that call is replaced. Everything the player can
		// activate goes through it, scripted or not, so the fish are stopped before any of their scripts hear about it.
		REL::Relocation<std::uintptr_t> pick{ RELOCATION_ID(39471, 40548) };  // PlayerCharacter::ActivatePickRef
		REL::Relocation<std::uintptr_t> act{ RELOCATION_ID(19369, 19796) };   // TESObjectREFR::ActivateRef
		const auto                      start = pick.address();
		std::uintptr_t                  site = 0;
		int                             sites = 0;
		for (std::uintptr_t off = 0; off < 0x600; ++off) {
			const auto* p = reinterpret_cast<const std::uint8_t*>(start + off);
			if (*p == 0xE8) {
				const auto rel = *reinterpret_cast<const std::int32_t*>(p + 1);
				if (start + off + 5 + rel == act.address()) {
					site = start + off;
					++sites;
				}
			}
		}
		if (sites != 1) {
			SKSE::log::error("Hand fishing: found {} calls to ActivateRef in ActivatePickRef (expected 1), not hooked", sites);
			return;
		}
		SKSE::AllocTrampoline(16);
		auto& trampoline = SKSE::GetTrampoline();
		Hook::func = trampoline.write_call<5>(site, Hook::Thunk);
		SKSE::log::info("Hand fishing: ActivatePickRef -> ActivateRef hooked (call at +{:#x})", site - start);
	}

	void OnDataLoaded()
	{
		SKSE::log::info("Hand fishing: {}, catch text \"{}\", fish model words \"{}\"", Settings::Get().blockHandFishing ? "on" : "off", Settings::Get().catchText,
			Settings::Get().fishModelWords);
	}
}
