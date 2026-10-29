"""Builds the Spriggit source of Fishing2026.esp: ONE override of the CC fishing quest (ccBGSSSE001_FishingSystemQuest, 033A57) whose script has the properties
of Fancy Fishing's copy plus the ones only Line and Lure's copy has, so the merged script finds every property filled.

  python tools/make_patch.py        reads yaml/fancy and yaml/llfp (Spriggit exports of the two plugins), writes yaml/patch
Then compile: bash tools/spriggit-cli.sh deserialize --InputPath yaml/patch --OutputPath <out>/Fishing2026.esp
"""
import glob
import os
import shutil

import yaml

HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
YAML = os.path.join(HERE, "yaml")
OUT = os.path.join(YAML, "patch")
NAME = "Fishing2026.esp"


def quest(folder):
    path = glob.glob(os.path.join(YAML, folder, "Quests", "ccBGSSSE001_FishingSystemQuest*"))[0]
    return path, yaml.safe_load(open(path, encoding="utf-8"))


fancy_path, fancy = quest("fancy")
_, llfp = quest("llfp")
fancy_script = fancy["VirtualMachineAdapter"]["Scripts"][0]
llfp_script = llfp["VirtualMachineAdapter"]["Scripts"][0]
assert fancy_script["Name"].lower() == llfp_script["Name"].lower() == "ccbgssse001_fishingsystemscript"
have = {p["Name"].lower() for p in fancy_script["Properties"]}
added = [p for p in llfp_script["Properties"] if p["Name"].lower() not in have]
fancy_script["Properties"].extend(added)
print(f"Fancy's {len(have)} properties + {len(added)} from Line and Lure = {len(fancy_script['Properties'])}")

if os.path.exists(OUT):
    shutil.rmtree(OUT)
os.makedirs(os.path.join(OUT, "Quests"))
dest = os.path.join(OUT, "Quests", os.path.basename(fancy_path))
with open(dest, "w", encoding="utf-8", newline="\n") as f:
    yaml.safe_dump(fancy, f, sort_keys=False, allow_unicode=True, default_flow_style=False, width=1000)

masters = ["Skyrim.esm", "Update.esm", "Dawnguard.esm", "HearthFires.esm", "Dragonborn.esm", "ccbgssse001-fish.esm", "FancyFishing.esp", "LLFP.esp"]
lines = ["SpriggitSource:", "  PackageName: Spriggit.Yaml.Skyrim", "  Version: 0.41", f"ModKey: {NAME}", "GameRelease: SkyrimSE", "ModHeader:", "  Flags:", "  - Small",
         "  Stats:", "    Version: 1.7", "  Author: CageTV",
         "  Description: Fishing 2026 - the CC fishing quest's script properties for the merged fishing script (Fancy Fishing + Simple Fishing Overhaul + Line and Lure).",
         "  MasterReferences:"]
for m in masters:
    lines += [f"  - Master: {m}", "    FileSize: 0"]
lines += ["  INTV: 1", ""]
open(os.path.join(OUT, "RecordData.yaml"), "w", encoding="utf-8", newline="\n").write("\n".join(lines))
open(os.path.join(OUT, "spriggit-meta.json"), "w", encoding="utf-8", newline="\n").write(
    '{\n  "PackageName": "Spriggit.Yaml.Skyrim",\n  "Version": "0.41.0",\n  "Release": "SkyrimSE",\n  "ModKey": "' + NAME + '"\n}\n')
print("wrote", OUT)
