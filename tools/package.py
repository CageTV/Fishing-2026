"""Lays out the Fishing 2026 package (staging in package/, nothing outside the workspace is touched).

  package/Fishing 2026 - Installer(.zip)   FOMOD: the plugin, scripts, skills-menu files and sky mesh, and the DLL build to pick (new / older)

Needs the compiled scripts in build/ (python tools/build_scripts.py), plugin/build/release/Fishing2026.dll (older, plugin/build.cmd) and
E:/WorkSpace/ng-build/build/release/out/Fishing2026/Fishing2026.dll (new, ng-build/build.cmd Fishing2026).
The fishing mods this builds on are requirements and are not packaged.

Usage: python tools/package.py [version]
"""
import os
import shutil
import sys
import xml.dom.minidom
import zipfile

HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
VERSION = sys.argv[1] if len(sys.argv) > 1 else "1.0.0"
DLL = "Fishing2026.dll"
OLD = os.path.join(HERE, "plugin", "build", "release", DLL)
NEW = os.path.join("E:\\", "WorkSpace", "ng-build", "build", "release", "out", "Fishing2026", DLL)
PEX = ["ccBGSSSE001_FishingActScript.pex", "FishAnywhereAliasScript.pex", "Fishing2026Native.pex"]
PSC = ["ccBGSSSE001_FishingActScript.psc", "FishAnywhereAliasScript.psc", "Fishing2026Native.psc"]
for p in (OLD, NEW) + tuple(os.path.join(HERE, "build", n) for n in PEX):
    assert os.path.exists(p), f"missing {p}"
NAME = "Fishing 2026"
NEW_VER = "1.6.1170.0"


def put(src, dst):
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    shutil.copyfile(src, dst)


dest = os.path.join(HERE, "package", NAME + " - Installer")
if os.path.exists(dest):
    shutil.rmtree(dest)
main = os.path.join(dest, "main")
put(os.path.join(HERE, "files", "Fishing2026.esp"), os.path.join(main, "Fishing2026.esp"))
for n in PEX:
    put(os.path.join(HERE, "build", n), os.path.join(main, "Scripts", n.lower() if n.lower().startswith("ccbgssse001") else n))
for n in PSC:
    put(os.path.join(HERE, "src", n), os.path.join(main, "Scripts", "Source", n))
for rel in ("SKSE/Plugins/CustomSkills/SKILLS.json", "SKSE/Plugins/CustomSkills/fishingskill.json", "SKSE/Plugins/CustomSkills/Fishing/Fishing.json",
            "meshes/Firmament/interface/INTPerkSkydome.nif", "Interface/fishing2026/fishing2026_logo.png"):
    put(os.path.join(HERE, "files", *rel.split("/")), os.path.join(main, *rel.split("/")))
put(NEW, os.path.join(dest, "new", "SKSE", "Plugins", DLL))
put(OLD, os.path.join(dest, "older", "SKSE", "Plugins", DLL))
put(os.path.join(HERE, "docs", "README.txt"), os.path.join(dest, "README.txt"))
put(os.path.join(HERE, "LICENSE.txt"), os.path.join(dest, "LICENSE.txt"))

config = f"""<config xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://qconsulting.ca/fo3/ModConfig5.0.xsd">
  <moduleName>{NAME}</moduleName>
  <requiredInstallFiles>
    <folder source="main" destination=""/>
    <file source="README.txt" destination="{NAME} README.txt"/>
    <file source="LICENSE.txt" destination="{NAME} LICENSE.txt"/>
  </requiredInstallFiles>
  <installSteps order="Explicit">
    <installStep name="Game version">
      <optionalFileGroups order="Explicit">
        <group name="{DLL}" type="SelectExactlyOne">
          <plugins order="Explicit">
            <plugin name="New (Skyrim 1.6.1170 and newer)">
              <description>The new build. Works on Skyrim SE/AE 1.6.1170 and on every later version, including 1.7.x. Pre-selected when the installer sees 1.6.1170 or newer.</description>
              <files><folder source="new" destination=""/></files>
              <typeDescriptor><dependencyType><defaultType name="Optional"/><patterns>
                <pattern><dependencies><gameDependency version="{NEW_VER}"/></dependencies><type name="Recommended"/></pattern>
              </patterns></dependencyType></typeDescriptor>
            </plugin>
            <plugin name="Older (Skyrim VR and 1.6.1130 and older)">
              <description>The older build, for Skyrim VR and SE/AE 1.6.1130 and older. On 1.6.1170 either build works. On VR the fish can still be caught by hand.</description>
              <files><folder source="older" destination=""/></files>
              <typeDescriptor><dependencyType><defaultType name="Optional"/><patterns>
                <pattern><dependencies><gameDependency version="{NEW_VER}"/></dependencies><type name="Optional"/></pattern>
              </patterns></dependencyType></typeDescriptor>
            </plugin>
          </plugins>
        </group>
      </optionalFileGroups>
    </installStep>
  </installSteps>
</config>
"""
info = (f"<fomod>\n  <Name>{NAME}</Name>\n  <Author>CageTV</Author>\n  <Version>{VERSION}</Version>\n"
        "  <Website>https://github.com/CageTV/Fishing-2026</Website>\n"
        "  <Description>Brings the Creation Club fishing mods together: the Angler skill in the vanilla skills menu, no catching fish by hand, fishing at shallow "
        "shores, one set of scripts, and Fancy Fishing's settings on an SKSE Menu Framework page.</Description>\n</fomod>\n")
xml.dom.minidom.parseString(config)
xml.dom.minidom.parseString(info)
os.makedirs(os.path.join(dest, "fomod"), exist_ok=True)
open(os.path.join(dest, "fomod", "ModuleConfig.xml"), "w", encoding="utf-8", newline=chr(10)).write(config)
open(os.path.join(dest, "fomod", "info.xml"), "w", encoding="utf-8", newline=chr(10)).write(info)
zpath = dest + ".zip"
if os.path.exists(zpath):
    os.remove(zpath)
with zipfile.ZipFile(zpath, "w", zipfile.ZIP_DEFLATED) as z:
    for dp, _, fs in os.walk(dest):
        for fn in fs:
            p = os.path.join(dp, fn)
            z.write(p, os.path.relpath(p, dest))
    names = z.namelist()
print(f"{zpath}: {len(names)} entries, {os.path.getsize(zpath) // 1024} KB")
for n in sorted(names):
    print("  ", n)
