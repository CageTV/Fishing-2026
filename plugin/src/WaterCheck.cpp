/*
 * Fishing 2026
 * Copyright (c) 2026 CageTV
 *
 * Released under the MIT License; see LICENSE.txt.
 */
#include "PCH.h"
#include "WaterCheck.h"
#include "Settings.h"

namespace WaterCheck
{
	namespace
	{
		constexpr float kNoWater = -1.0e8f;
		int             logged = 0;

		// Water surface height at a point, or kNoWater
		float WaterAt(RE::TES* a_tes, RE::TESObjectCELL* a_cell, const RE::NiPoint3& a_pos)
		{
			const float h = a_tes->GetWaterHeight(a_pos, a_cell);
			return (h > kNoWater && h < 1.0e7f) ? h : kNoWater;
		}

		// Walks away from the player along the way they face: the first point with water at least a_minDepth deep (water surface above the ground) is enough
		bool IsWaterAhead(RE::StaticFunctionTag*, RE::Actor* a_actor, float a_maxDistance, float a_minDepth)
		{
			if (!a_actor) {
				return false;
			}
			auto* tes = RE::TES::GetSingleton();
			auto* cell = a_actor->GetParentCell();
			if (!tes || !cell) {
				return false;
			}
			const auto  pos = a_actor->GetPosition();
			const float yaw = a_actor->GetAngleZ();  // radians, 0 = north (+y), clockwise
			const float dx = std::sin(yaw), dy = std::cos(yaw);
			const bool  interior = cell->IsInteriorCell();
			const float step = 35.0f;  // half a metre
			float       bestDepth = -1.0f, bestAt = 0.0f;
			std::string trace;
			for (float d = 35.0f; d <= a_maxDistance; d += step) {
				const RE::NiPoint3 p{ pos.x + dx * d, pos.y + dy * d, pos.z };
				const float        water = WaterAt(tes, cell, p);
				if (water <= kNoWater) {
					continue;
				}
				float depth;
				float land = 0.0f;
				if (!interior && tes->GetLandHeight(p, land)) {
					depth = water - land;
				} else if (interior) {
					depth = water - (pos.z - 150.0f);  // no terrain indoors: the water has to lie within a body-length below the feet
				} else {
					continue;
				}
				if (depth > bestDepth) {
					bestDepth = depth;
					bestAt = d;
				}
				if (depth >= a_minDepth) {
					if (logged < 30) {
						++logged;
						SKSE::log::info("Water check: water {:.0f} units deep, {:.0f} units ahead (water {:.0f}, ground {:.0f}, feet {:.0f}): yes", depth, d, water, land, pos.z);
					}
					return true;
				}
			}
			if (logged < 30) {
				++logged;
				SKSE::log::info("Water check: no water {:.0f}+ deep within {:.0f} units (deepest seen {:.0f} at {:.0f}): no", a_minDepth, a_maxDistance, bestDepth, bestAt);
			}
			return false;
		}

		bool Register(RE::BSScript::IVirtualMachine* a_vm)
		{
			a_vm->RegisterFunction("IsWaterAhead", "Fishing2026Native", IsWaterAhead);
			return true;
		}
	}

	void RegisterPapyrus()
	{
		SKSE::GetPapyrusInterface()->Register(Register);
	}
}
