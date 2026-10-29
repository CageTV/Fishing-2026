/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#include <spdlog/sinks/basic_file_sink.h>

#include <Windows.h>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std::literals;

// One on-screen message, whichever way this CommonLib spells it.
inline void Notify(const char* a_text)
{
#if __has_include(<RE/S/SendHUDMessage.h>)
	RE::SendHUDMessage::ShowHUDMessage(a_text);
#else
	RE::DebugNotification(a_text);
#endif
}
