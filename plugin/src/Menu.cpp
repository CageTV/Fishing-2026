/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "FancySettings.h"
#include "Menu.h"
#include "Preview.h"
#include "Settings.h"

#include "SKSEMenuFramework.h"

namespace Menu
{
	namespace
	{
		using namespace ImGuiMCP;

		constexpr const char* kLogoPath = "Data/Interface/fishing2026/fishing2026_logo.png";

		void Logo()
		{
			static ImTextureID tex = SKSEMenuFramework::LoadTexture(kLogoPath);
			if (!tex) {
				return;
			}
			const float avail = GetContentRegionAvail().x;
			const float w = std::min(avail, 150.0f);
			SetCursorPosX(GetCursorPosX() + (avail - w) * 0.5f);
			Image(tex, ImVec2(w, w * 676.0f / 679.0f));
			Spacing();
		}

		void Tip(const char* a_text)
		{
			if (a_text && a_text[0] && IsItemHovered()) {
				BeginTooltip();
				PushTextWrapPos(420.0f);
				TextUnformatted(a_text);
				PopTextWrapPos();
				EndTooltip();
			}
		}

		// One page's rows of Fancy Fishing settings, grouped
		void RenderFancyPage(Fancy::Page a_page)
		{
			if (!Fancy::Available()) {
				TextColored(ImVec4(1.0f, 0.45f, 0.4f, 1.0f), "FancyFishing.esp is not in the load order.");
				return;
			}
			std::size_t count = 0;
			const auto* table = Fancy::Table(count);
			const char* group = nullptr;
			for (std::size_t i = 0; i < count; ++i) {
				const auto& e = table[i];
				if (e.page != a_page) {
					continue;
				}
				if (!group || std::string_view(group) != e.group) {
					group = e.group;
					Spacing();
					SeparatorText(group);
				}
				PushID(e.id);
				const float v = Fancy::Get(e);
				if (e.kind == Fancy::Kind::Toggle) {
					bool b = v != 0.0f;
					if (Checkbox(e.label, &b)) {
						Fancy::Set(e, b ? 1.0f : 0.0f);
					}
				} else if (e.kind == Fancy::Kind::Int) {
					int n = static_cast<int>(std::lround(v));
					if (SliderInt(e.label, &n, static_cast<int>(e.min), static_cast<int>(e.max))) {
						Fancy::Set(e, static_cast<float>(n));
					}
				} else {
					float f = v;
					if (SliderFloat(e.label, &f, e.min, e.max, "%.2f")) {
						Fancy::Set(e, f);
					}
				}
				Tip(e.tip);
				PopID();
			}
			Spacing();
			Separator();
			if (Button("Reset this page to the defaults")) {
				Fancy::ResetPage(a_page);
			}
		}

		// Drawn every frame whether or not the menu is open.
		void __stdcall Frame()
		{
			Preview::Draw();
		}

		void __stdcall RenderFishing()
		{
			Logo();
			SeparatorText("Fishing 2026");
			auto& s = Settings::Get();
			bool  block = s.blockHandFishing;
			if (Checkbox("Fish cannot be caught by hand", &block)) {
				s.blockHandFishing = block;
				Settings::Save();
			}
			Tip("Swimming fish (salmon, pond fish, Improved Fish's fish, the bucket and tripod fish) cannot be grabbed: you need a fishing rod. Dead and cooked fish "
				"can still be picked up. Settings: Data/SKSE/Plugins/Fishing2026/settings.ini.");
			RenderFancyPage(Fancy::Page::Fishing);
			Spacing();
			Separator();
			bool apply = Fancy::ApplyOnLoad();
			if (Checkbox("Apply these settings at the start of every game and load", &apply)) {
				Fancy::ApplyOnLoad() = apply;
				std::size_t count = 0;
				const auto* t = Fancy::Table(count);
				if (count) {
					Fancy::Set(t[0], Fancy::Get(t[0]));  // writes fancy.ini with the new switch
				}
			}
			Tip("The values on these pages are remembered in Data/SKSE/Plugins/Fishing2026/fancy.ini and written into the game each time, so they hold for every "
				"character. Switch off to let Fancy Fishing's own MCM page and each save decide instead.");
		}

		void __stdcall RenderOdds()
		{
			Logo();
			RenderFancyPage(Fancy::Page::Odds);
		}

		void __stdcall RenderMinigame()
		{
			Logo();
			Preview::Request();
			Checkbox("Dim the screen in the preview", &Preview::DimScreen());
			Tip("Shows how dark the screen behind the minigame gets (Background opacity).");
			RenderFancyPage(Fancy::Page::Minigame);
			Spacing();
			TextDisabled("While this page is open the minigame's position and size are drawn on screen as an outline. The minigame itself is Fancy Fishing's Flash widget "
						 "(Interface/exported/widgets/fancyfishing), so it can be reskinned.");
		}
	}

	void Register()
	{
		if (!SKSEMenuFramework::IsInstalled()) {
			SKSE::log::warn("SKSE Menu Framework is not installed: the Fishing 2026 pages are not available");
			return;
		}
		SKSEMenuFramework::SetSection("Fishing 2026");
		SKSEMenuFramework::AddSectionItem("Fishing", RenderFishing);
		SKSEMenuFramework::AddSectionItem("Catch odds", RenderOdds);
		SKSEMenuFramework::AddSectionItem("Minigame", RenderMinigame);
		SKSEMenuFramework::AddHudElement(Frame);
		SKSE::log::info("Registered with SKSE Menu Framework {}", SKSEMenuFramework::GetMenuFrameworkVersion());
	}
}
