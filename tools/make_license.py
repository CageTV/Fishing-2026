"""Builds LICENSE.txt from Hunting in Skyrim 2026's (same MIT text, GPL notice for the "new" DLL and library notices), with this project's own header and credits.

  python tools/make_license.py
"""
import os

HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
src = open(r"E:/WorkSpace/Hunting-in-Skyrim-2026/LICENSE.txt", encoding="utf-8", newline="").read().replace(chr(13) + chr(10), chr(10))
# everything from the MIT text on is shared; the header (title, scope, credits) is ours
start = src.index("Permission is hereby granted")
body = src[start:]
header = """Fishing 2026
Copyright (c) 2026 CageTV

Released under the MIT License. Source code: https://github.com/CageTV/Fishing-2026

This license covers the files written for this repository: Fishing2026.dll and its settings pages, Fishing2026Native.psc, the changes made to the two
scripts below, the Fishing 2026 plugin (an override of the Creation Club fishing quest), the skills-menu sky mesh built from Firmament's, and the build
tools. It does not cover plugin/include/SKSEMenuFramework.h (the SKSE Menu Framework 3 project's SDK header, under that project's terms).

CREDITS - the work this builds on, used with its authors' permission and credited here and on the download page
  Creation Club Fishing, by Bethesda Game Studios. A REQUIREMENT: the fishing quest, scripts, items and plugin are Bethesda's and are not included.
  Fish Anywhere With Water, by wankingSkeever. Its fishing-spot script and the script that starts fishing away from the usual spots are changed here (the
    water test is replaced by one in Fishing2026.dll). A REQUIREMENT: its plugin, activators and projectile are not included.
  Streamlined Fishing, by Parapets, and Fish Anywhere - Streamlined Fishing (Patch), by wankingSkeever and Parapets. The fishing-spot script here is the one
    those two put together, calling the rod choice only when Streamlined Fishing is installed. Optional; its plugin and scripts are not included.
  Immersive Fishing, by GiraPomba. Its fishing script is the one the game runs, and the rod and caught fish shown in the hands. A REQUIREMENT: not included.
  Fancy Fishing - Minigame and Tweaks, by LeucisticDinosaur. The minigame, its widget and its settings. A REQUIREMENT: not included. The settings pages in
    Fishing2026.dll write the settings its plugin keeps; its widget is untouched.
  Line and Lure - Fishing Perks, by Hydraaawr. The Angler perks and their tree (Fishing/Fishing.json is its skill and tree, with the skill renamed Angler),
    and the book that opens them. A REQUIREMENT: its plugin, scripts and sky model are not included.
  Simple Fishing Overhaul, by JaySerpa. The animations and the base of the fishing script. A REQUIREMENT: not included.
  Firmament, by JPSteel2, and Constellations - Additional Player Skills. The skills menu and its sky; INTPerkSkydome.nif here is Firmament's with one more
    constellation slot. Requirements: not included, apart from that mesh.
  Custom Skills Framework, by meh321 and Parapets. A REQUIREMENT: not included.
  The original authors keep the copyright in their work; it is used under the permission given, and it is not re-licensed.

The files written for this repository are released under the MIT License below, and the "new" Fishing2026.dll is also under the GNU General Public
License as described at the end of this file.

"""
text = (header + body).replace("HuntingInSkyrimNative.dll", "Fishing2026.dll").replace("Hunting in Skyrim 2026", "Fishing 2026").replace(
    "Hunting-in-Skyrim-2026", "Fishing-2026").replace("StressAndFear.dll", "Fishing2026.dll").replace("Stress and Fear 2026", "Fishing 2026").replace(
    "Stress-and-Fear-2026", "Fishing-2026")
open(os.path.join(HERE, "LICENSE.txt"), "w", encoding="utf-8", newline="\n").write(text)
print("LICENSE.txt:", len(text), "chars")
