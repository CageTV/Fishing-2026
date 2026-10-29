# Fishing 2026

Fishing for the Creation Club Fishing content, brought together in one place. It joins the fishing mods you already use so they work together, and adds an SKSE plugin (`Fishing2026.dll`) that does what they could not.

- **Angler**: Line and Lure's fishing skill and perks in the vanilla skills menu (25 constellations), opened on Angler by the Line and Lure book.
- **No catching fish by hand**: swimming fish need a rod; dead and cooked fish can still be picked up.
- **Fishing away from the usual spots** works at shallow shores, docks and bridges: the test for water ahead is done natively.
- **One set of fishing scripts**: Immersive Fishing's (Fancy Fishing + Line and Lure + Simple Fishing Overhaul + the rod and fish in your hands), plus Fish Anywhere's fishing-spot script with Streamlined Fishing's rod choice.
- **Fancy Fishing's minigame on by default**, with all its settings on an SKSE Menu Framework page, remembered across games, and an on-screen outline of the minigame while you place and size it. The minigame widget itself is untouched and can be reskinned.

Requirements, install order, settings and credits are in [docs/README.txt](docs/README.txt) (it ships inside the download).

## Build

- `plugin/` - the DLL. New build (alandtse's CommonLibSSE-NG, Skyrim 1.6.1170+ and 1.7.x) from [ng-build](plugin/ng.cmake); older build (CharmedBaryon's, VR and 1.6.1130 and older) with `plugin/build.cmd`.
- `src/` - the Papyrus scripts: `python tools/build_scripts.py` (needs the game's script sources and the mods listed in the script).
- `tools/build_skydome.py` - the skills-menu sky: `--points N` makes a sky for N skills (the menu turns the camera by 360/N degrees per skill, so the sky needs exactly as many slots as skills).
- `tools/package.py` - the FOMOD installer.

Licensed as described in [LICENSE.txt](LICENSE.txt).
