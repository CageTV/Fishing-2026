"""Compile the merged fishing script with the official Papyrus compiler.

  python tools/build_scripts.py     src/*.psc -> build/

Imports: the script being built, compile-time stubs for optional mods, Line and Lure's and Fancy Fishing's own sources (their quest/MCM scripts are
referenced by property types), Custom Skills Framework, SKSE, then the game's sources (which include the CC fishing scripts).
"""
import os
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GAME = r"E:\Tabula Rasa\Stock Game"
MODS = r"C:\Modlists\Tabula\mods"
SRC = os.path.join(HERE, "src")
OUT = os.path.join(HERE, "build")
COMPILER = os.path.join(GAME, "Papyrus Compiler", "PapyrusCompiler.exe")
FLAGS = os.path.join(GAME, "Data", "Source", "Scripts", "TESV_Papyrus_Flags.flg")
IMPORTS = [
    SRC,
    os.path.join(HERE, "stubs"),
    os.path.join(MODS, "Line and Lure - Fishing Perks", "Scripts", "Source"),
    os.path.join(MODS, "Fancy Fishing", "scripts", "source"),
    os.path.join(MODS, "Custom Skills Framework", "Source", "Scripts"),
    os.path.join(MODS, "Streamlined Fishing", "Source", "Scripts"),
    os.path.join(MODS, "Fish Anywhere With Water", "Source", "Scripts"),
    os.path.join(MODS, "Fishing Preview", "Source", "Scripts"),
    os.path.join(MODS, "powerofthree's Papyrus Extender", "Source", "scripts"),
    os.path.join(MODS, "Skyrim Script Extender (SKSE64) 203", "Scripts", "Source"),
    os.path.join(GAME, "Data", "Source", "Scripts"),
]
for p in [COMPILER, FLAGS, SRC] + IMPORTS:
    if not os.path.exists(p):
        raise SystemExit(f"missing: {p}")
if os.path.exists(OUT):
    shutil.rmtree(OUT)
os.makedirs(OUT)
names = sorted(n for n in os.listdir(SRC) if n.lower().endswith(".psc"))
log, rc = "", 0
for name in names:
    r = subprocess.run([COMPILER, name, f"-f={FLAGS}", "-i=" + ";".join(IMPORTS), f"-o={OUT}"], capture_output=True, text=True, cwd=SRC, timeout=180)
    log += (r.stdout or "") + (r.stderr or "")
    rc = rc or r.returncode
open(os.path.join(OUT, "compile.log"), "w", encoding="utf-8").write(log)
errors = [l for l in log.splitlines() if ".psc(" in l]
print(f"compiler exit {rc}: {len([n for n in os.listdir(OUT) if n.endswith('.pex')])} .pex; {len(errors)} error lines")
for l in errors[:30]:
    print("  " + l)
sys.exit(0 if rc == 0 and not errors else 1)
