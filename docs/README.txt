Fishing 2026
Fishing for the Creation Club Fishing content, brought together in one place. Part of the 2026 series with Campfire 2026, Frostfall 2026, Last Seed 2026,
Hunterborn 2026, Hunting in Skyrim 2026 and more.

This is not a new fishing mod. It joins the fishing mods you already use so they work together, and adds an SKSE plugin that does what they could not.
The mods below stay installed; they are requirements.

WHAT IT DOES (1.0.0)
--------------------
- Angler: Line and Lure's fishing skill and perks live in the vanilla skills menu, as the 25th constellation, and the Line and Lure book opens the menu
  on it. The skill is called Angler. The sky mesh has one more constellation slot than Firmament's (25 in all), with the fish as its outline.
- Fish cannot be caught by hand. The swimming fish (salmon, pond fish, Improved Fish's fish, the bucket and tripod fish) cannot be grabbed: you need a
  fishing rod. Dead and cooked fish can still be picked up. Turn it off on the Fishing page.
- Fishing away from the usual spots (Fish Anywhere With Water) works at shallow shores, docks and bridges. The test for water in front of you is done by
  Fishing2026.dll, which looks at the water surface and the ground under it, instead of a projectile and a hidden NPC.
- One set of fishing scripts. Immersive Fishing's script (Fancy Fishing + Line and Lure + Simple Fishing Overhaul + the rod and fish shown in your hands)
  is the one that runs; this plugin fills in the properties it needs. The fishing-spot script is Fish Anywhere's with Streamlined Fishing's rod choice
  (called only when Streamlined Fishing is installed), so the separate "Fish Anywhere - Streamlined Fishing (Patch)" is not needed.
- Fancy Fishing's minigame is on by default. A "Fishing 2026" section in SKSE Menu Framework has the pages Fishing, Catch odds and Minigame with all of Fancy
  Fishing's settings, remembered in Data/SKSE/Plugins/Fishing2026/fancy.ini and applied at the start of every game and load. While the Minigame page is open
  an outline of the minigame shows where it will be and how big. The minigame itself is Fancy Fishing's Flash widget, untouched, so it can be reskinned.

REQUIREMENTS
------------
Skyrim SE/AE (1.6.1170 and later, including 1.7.x, with the "new" DLL; the "older" DLL is for Skyrim VR and 1.6.1130 and older - the hand-fishing block does
not work on VR), SKSE, Address Library for SKSE Plugins.
  Fishing (Creation Club) - ccBGSSSE001-Fish.esm
  Custom Skills Framework (Nexus 41780)
  Constellations - Additional Player Skills (117352) and Firmament (153781): this sky replaces Firmament's
  Line and Lure - Fishing Perks (145240) and Simple Fishing Overhaul - Animations and Improved Quest Dialogue (103440)
  Immersive Fishing (115249), with Open Animation Replacer and Immersive Equipment Displays
  Fancy Fishing - Minigame and Tweaks (89352)
  Fish Anywhere With Water (60915), with powerofthree's Papyrus Extender
  SKSE Menu Framework 3, for the settings pages (the rest works without it)
Optional: Streamlined Fishing (80683), Fishing Preview (107476), Improved Fish SE (17324).

INSTALL
-------
Install the fishing mods above first, then this with your mod manager, and pick the DLL build for your game (the installer pre-selects it).
Put "Fishing 2026" ABOVE Immersive Fishing, Fancy Fishing, Line and Lure, Simple Fishing Overhaul, Fish Anywhere With Water and Streamlined Fishing in the
mod list (it must win over Firmament for the sky, and over Fish Anywhere for its scripts), and Fishing2026.esp AFTER them in the plugin list.
Keep the order the fishing mods ask for among themselves: Simple Fishing Overhaul, then Line and Lure, with Fancy Fishing and Fish Anywhere after.

You do not need LLFP_SFO_patch.esp or "Fish Anywhere - Streamlined Fishing (Patch)": turn them off.

SETTINGS
--------
SKSE Menu Framework > Fishing 2026. Hand fishing, auto-reel, bite time and the rest on Fishing; the chances of rare and junk catches on Catch odds; the
minigame's size, position, speed and difficulty on Minigame. Key re-mapping for the minigame stays on Fancy Fishing's own MCM page. The files are in
Data/SKSE/Plugins/Fishing2026/: settings.ini (hand fishing: the message, the prompt text and model words that make a thing a fish) and fancy.ini.
A log is written to the SKSE logs folder as Fishing2026.log.

KNOWN
-----
- The minigame outline is a stand-in: it is placed and sized like the real widget but is not its art, and the catch meter is not drawn.
- Water that damages you (lava) is not told apart from water for fishing.
- The sky has exactly 25 constellations. A 26th skill needs the mesh rebuilt (tools/build_skydome.py --points 26): the menu turns the camera by 360 divided by
  the number of skills, so the sky must have as many slots as there are skills.

CREDITS
-------
Creation Club Fishing by Bethesda Game Studios. Fish Anywhere With Water by wankingSkeever. Streamlined Fishing by Parapets, and its Fish Anywhere patch by
wankingSkeever and Parapets. Immersive Fishing by GiraPomba. Fancy Fishing by LeucisticDinosaur. Line and Lure - Fishing Perks by Hydraaawr. Simple Fishing
Overhaul by JaySerpa. Firmament by JPSteel2 and Constellations. Custom Skills Framework by meh321 and Parapets. Settings pages and Fishing2026.dll by CageTV.
All used with their authors' permission; the originals stay theirs. Source: https://github.com/CageTV/Fishing-2026
