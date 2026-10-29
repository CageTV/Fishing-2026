/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "FancySettings.h"
#include "HandFishing.h"
#include "Menu.h"
#include "Settings.h"
#include "WaterCheck.h"

namespace
{
	void SetupLog()
	{
		const auto dir = SKSE::log::log_directory();
		if (!dir) {
			return;
		}
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>((*dir / "Fishing2026.log").string(), true);
		auto log = std::make_shared<spdlog::logger>("Fishing2026", std::move(sink));
		log->set_level(spdlog::level::info);
		log->flush_on(spdlog::level::info);
		log->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
		spdlog::set_default_logger(std::move(log));
	}

	void OnMessage(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kPostLoad:
			Menu::Register();  // SKSE Menu Framework loads after us alphabetically, so register once everything is loaded
			break;
		case SKSE::MessagingInterface::kDataLoaded:
			HandFishing::OnDataLoaded();
			Fancy::Init();
			break;
		case SKSE::MessagingInterface::kNewGame:
		case SKSE::MessagingInterface::kPostLoadGame:
			Fancy::ApplyAll();
			break;
		default:
			break;
		}
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);
	SetupLog();
	SKSE::log::info("Fishing2026.dll 1.0.0, game {}", REL::Module::get().version().string());
	Settings::Load();
	HandFishing::Install();
	WaterCheck::RegisterPapyrus();
	SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
	return true;
}
