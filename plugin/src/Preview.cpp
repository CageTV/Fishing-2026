/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "FancySettings.h"
#include "Preview.h"

#include "SKSEMenuFramework.h"

namespace Preview
{
	namespace
	{
		using namespace ImGuiMCP;

		std::chrono::steady_clock::time_point lastRequest{};
		bool                                  dim = false;

		inline ImU32 Col(float r, float g, float b, float a)
		{
			return IM_COL32(static_cast<int>(std::clamp(r, 0.0f, 255.0f)), static_cast<int>(std::clamp(g, 0.0f, 255.0f)), static_cast<int>(std::clamp(b, 0.0f, 255.0f)),
				static_cast<int>(std::clamp(a, 0.0f, 1.0f) * 255.0f));
		}

		float Value(const char* a_id, float a_fallback)
		{
			std::size_t count = 0;
			const auto* table = Fancy::Table(count);
			for (std::size_t i = 0; i < count; ++i) {
				if (std::string_view(table[i].id) == a_id) {
					return Fancy::Get(table[i]);
				}
			}
			return a_fallback;
		}
	}

	bool& DimScreen() { return dim; }

	void Request() { lastRequest = std::chrono::steady_clock::now(); }

	void Draw()
	{
		using namespace std::chrono;
		if (steady_clock::now() - lastRequest > milliseconds(250) || !Fancy::Available()) {
			return;
		}
		auto* io = GetIO();
		auto* dl = GetForegroundDrawList();
		if (!io || !dl) {
			return;
		}
		const float W = io->DisplaySize.x, H = io->DisplaySize.y;
		const float k = std::min(W / 1280.0f, H / 720.0f);  // the Flash stage fits the screen
		const float ox = (W - 1280.0f * k) * 0.5f, oy = (H - 720.0f * k) * 0.5f;
		const float scale = Value("GlobalScale", 0.67f);
		const float cx = 640.0f + Value("CenterX", 0.0f), cy = 360.0f + Value("CenterY", 0.0f);
		const float bgdAlpha = Value("BgdAlpha", 0.5f);
		const float barMin = Value("ScaleBarMin", 0.2f), barMax = Value("ScaleBarMax", 0.2f);
		const float barOff = Value("BarAlphaOff", 0.4f);
		const bool  perfect = Value("ShowPerfect", 1.0f) != 0.0f;

		auto P = [&](float sx, float sy) { return ImVec2(ox + (cx + sx * scale) * k, oy + (cy + sy * scale) * k); };

		if (dim) {
			ImDrawListManager::AddRectFilled(dl, ImVec2(ox, oy), ImVec2(ox + 1280.0f * k, oy + 720.0f * k), Col(0, 0, 0, bgdAlpha), 0.0f, 0);
		}
		// frame 128 x 600 centred on the minigame's centre
		ImDrawListManager::AddRectFilled(dl, P(-64, -300), P(64, 300), Col(20, 24, 30, 0.35f), 4.0f * k, 0);
		ImDrawListManager::AddRect(dl, P(-64, -300), P(64, 300), Col(215, 190, 120, 0.95f), 4.0f * k, 0, 2.0f);
		// the catch bar: 46 wide, 600 * bar size tall (this is the start size; the dashed outline is the size it grows to)
		const float barH = 600.0f * barMin;
		const float barTop = 60.0f;
		ImDrawListManager::AddRectFilled(dl, P(-23, barTop), P(23, barTop + barH), Col(120, 210, 140, 0.55f), 2.0f * k, 0);
		ImDrawListManager::AddRect(dl, P(-23, barTop), P(23, barTop + barH), Col(150, 235, 170, 0.95f), 2.0f * k, 0, 2.0f);
		if (barMax > barMin + 0.001f) {
			const float bigH = 600.0f * barMax;
			ImDrawListManager::AddRect(dl, P(-27, barTop - (bigH - barH) * 0.5f), P(27, barTop + barH + (bigH - barH) * 0.5f), Col(150, 235, 170, 0.45f), 2.0f * k, 0, 1.0f);
		}
		// the same bar as it looks while the fish is escaping
		ImDrawListManager::AddRectFilled(dl, P(26, barTop), P(32, barTop + barH), Col(120, 210, 140, barOff), 1.0f * k, 0);
		// the fish: 64 x 64
		ImDrawListManager::AddEllipseFilled(dl, P(0, barTop + barH * 0.5f - 10), ImVec2(30.0f * scale * k, 18.0f * scale * k), Col(110, 170, 230, 0.95f), 0.0f, 24);
		if (perfect) {
			ImDrawListManager::AddText(dl, P(-30, -330), Col(255, 235, 140, 0.95f), "Perfect!");
		}
		ImDrawListManager::AddText(dl, P(-64, 310), Col(255, 255, 255, 0.85f), "Fishing 2026: placement preview");
		ImDrawListManager::AddText(dl, P(-64, 330), Col(255, 255, 255, 0.6f), "(the real widget keeps its own look)");
	}
}
