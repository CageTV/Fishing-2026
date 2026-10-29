"""Builds the expanded skills-menu skydome: Firmament's INTPerkSkydome.nif with room for more than its 24 constellations.

  python tools/build_skydome.py [--points 35] [--radius 99.34] [--out build/meshes/Firmament/interface/INTPerkSkydome.nif]

The skills menu looks the sky up by name: PointNN is where constellation NN sits (the menu reads its world position and the camera flies there)
and cImageNN is the golden outline picture of that constellation. A skill without a PointNN crashes the menu when it is left, so every skill
needs a slot. This takes the Firmament skydome and
  1. re-spaces the ring to `points` evenly spaced slots (Constellations' skydome is laid out the same way: radius 99.34, slot k at 90 - k*step degrees,
     rotated -k*step about z), moving each outline picture with its slot,
  2. adds the missing PointNN nodes and cImageNN outline pictures,
  3. gives slot 24 the fishing constellation's outline (Line and Lure's picture), the other new slots an invisible placeholder picture.

Needs PyNifly (tools/pynifly in the toolkit) and Python 3.
"""
import argparse
import math
import os
import sys
from ctypes import byref

TOOLKIT = r"E:\Tabula Rasa\mods\Skyrim-Claude Code Modder's Toolkit"
MODS = r"C:\Modlists\Tabula\mods"
sys.path.insert(0, os.path.join(TOOLKIT, "tools", "pynifly", "io_scene_nifly"))
from pyn import pynifly  # noqa: E402
from pyn.pynifly import nifly  # noqa: E402

pynifly.NifFile.Load(os.path.join(TOOLKIT, "tools", "pynifly", "io_scene_nifly", "pyn", "NiflyDLL.dll"))

BASE = os.path.join(MODS, "Firmament", "meshes", "Firmament", "interface", "INTPerkSkydome.nif")
FISH = os.path.join(MODS, "Line and Lure - Fishing Perks", "meshes", "LLFP", "interface", "intllfpskydome.nif")
FISH_SLOT = 24


def rz(deg):
    a = math.radians(deg)
    c, s = math.cos(a), math.sin(a)
    return [[c, -s, 0.0], [s, c, 0.0], [0.0, 0.0, 1.0]]


def mat_vec(m, v):
    return tuple(sum(m[i][j] * v[j] for j in range(3)) for i in range(3))


def mat_mat(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(3)) for j in range(3)] for i in range(3)]


def get_rot(t):
    return [[t.rotation[i][j] for j in range(3)] for i in range(3)]


def set_rot(t, m):
    for i in range(3):
        for j in range(3):
            t.rotation[i][j] = m[i][j]


def new_transform(template, pos, rot, scale):
    t = type(template).from_buffer_copy(template)
    for i in range(3):
        t.translation[i] = pos[i]
    set_rot(t, rot)
    t.scale = scale
    return t


def write_node(nf, node):
    """Writes a node's changed properties (its transform) back into the file."""
    check = nifly.setBlock(nf._handle, node.id, byref(node.properties))
    return check


def slot_pos(k, step, radius, z):
    ang = math.radians(90.0 - k * step)
    return (radius * math.cos(ang), radius * math.sin(ang), z)


def card_world(shape):
    t = shape.transform
    rot, sc, tr = get_rot(t), t.scale, t.translation
    return [tuple(sc * c + tr[i] for i, c in enumerate(mat_vec(rot, v))) for v in shape.verts], rot


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--points", type=int, default=35)
    ap.add_argument("--radius", type=float, default=99.34)
    ap.add_argument("--out", default=os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "build", "meshes", "Firmament", "interface", "INTPerkSkydome.nif"))
    args = ap.parse_args()
    n, radius = args.points, args.radius
    step = 360.0 / n
    os.makedirs(os.path.dirname(args.out), exist_ok=True)

    nf = pynifly.NifFile(BASE)
    nf.filepath = args.out
    opn = nf.nodes["OrderedRenderingNode"]
    old_points = sorted(k for k in nf.nodes if k.startswith("Point"))
    old_count = len(old_points)
    if old_count > n:
        sys.exit(f"the base skydome already has {old_count} points, more than --points {n}")
    z = nf.nodes["Point00"].transform.translation[2]
    scale = nf.nodes["Point00"].transform.scale
    template = nf.nodes["Point00"].transform

    # remember where each old point was before it moves, so its picture keeps its place relative to it
    old_angle = {}
    for k in range(old_count):
        x, y, _ = nf.nodes[f"Point{k:02d}"].transform.translation
        old_angle[k] = math.degrees(math.atan2(y, x))

    # 1. the ring: existing slots move, missing ones are made
    for k in range(n):
        name = f"Point{k:02d}"
        xform = new_transform(template, slot_pos(k, step, radius, z), rz(-k * step), scale)
        if k < old_count:
            node = nf.nodes[name]
            node.transform = xform
            write_node(nf, node)
        else:
            nf.add_node(name, xform, opn)
    print(f"ring: {n} slots, {step:.2f} degrees apart, radius {radius}, {old_count} existing + {n - old_count} new")

    # 2. the existing outline pictures follow their slots
    shapes = {s.name: s for s in nf.shapes}
    for k in range(old_count):
        s = shapes.get(f"cImage{k:02d}")
        if s is None:
            sys.exit(f"cImage{k:02d} not found in the base skydome")
        delta = (90.0 - k * step) - old_angle[k]
        t = s.transform
        pos = mat_vec(rz(delta), tuple(t.translation))
        rot = mat_mat(rz(delta), get_rot(t))
        s.transform = new_transform(t, pos, rot, t.scale)

    # 3. outline pictures for the new slots
    fish = pynifly.NifFile(FISH)
    fs = next(s for s in fish.shapes if s.name == "cImage00")
    fworld, frot = card_world(fs)
    fnorm = [mat_vec(frot, nv) for nv in fs.normals]
    cx = sum(p[0] for p in fworld) / len(fworld)
    cy = sum(p[1] for p in fworld) / len(fworld)
    cz = sum(p[2] for p in fworld) / len(fworld)
    f_angle = math.degrees(math.atan2(cy, cx))
    f_rad = math.hypot(cx, cy)
    # size: the tangential width of the fish card, brought to a size that sits well beside the other constellations
    tx, ty = -math.sin(math.radians(f_angle)), math.cos(math.radians(f_angle))
    tw = [p[0] * tx + p[1] * ty for p in fworld]
    fish_width = max(tw) - min(tw)
    target_width = 26.0
    fish_scale = target_width / fish_width if fish_width > 0 else 1.0
    print(f"fish card: width {fish_width:.1f} -> {target_width}, scale {fish_scale:.3f}, at {f_angle:.1f} deg, radius {f_rad:.1f}, z {cz:.1f}")

    ref_shader = fs.shader
    ref_alpha = fs.alpha_property
    identity = new_transform(template, (0.0, 0.0, 0.0), rz(0.0), 1.0)

    def add_card(name, k, visible):
        delta = (90.0 - k * step) - f_angle
        rm = rz(delta)
        sc = fish_scale if visible else 0.0005
        phi = math.radians(90.0 - k * step)
        centre = (radius * 0.93 * math.cos(phi), radius * 0.93 * math.sin(phi), 12.0)  # the other cards sit about 7% inside the ring, about 12 units up
        verts, norms = [], []
        for p, nv in zip(fworld, fnorm):
            rel = mat_vec(rm, ((p[0] - cx) * sc, (p[1] - cy) * sc, (p[2] - cz) * sc))
            verts.append((centre[0] + rel[0], centre[1] + rel[1], centre[2] + rel[2]))
            norms.append(mat_vec(rm, nv))
        shape = nf.createShapeFromData(name, verts, [tuple(t) for t in fs.tris], [tuple(u) for u in fs.uvs], norms, parent=opn)
        shape.transform = identity
        # shader and alpha: the fish card's own (an effect shader showing its picture), copied
        sh = pynifly.NiShader(file=nf, properties=type(ref_shader.properties).from_buffer_copy(ref_shader.properties), parent=shape)
        sh._name = ref_shader.name or ""
        shape._shader = sh
        shape.save_shader_attributes()
        if ref_alpha is not None:
            al = pynifly.NiAlphaProperty(file=nf, properties=type(ref_alpha.properties).from_buffer_copy(ref_alpha.properties), parent=shape)
            shape._alpha = al
            shape.save_alpha_property()
        return shape

    for k in range(old_count, n):
        add_card(f"cImage{k:02d}", k, visible=(k == FISH_SLOT))
    nf.save()
    print("wrote", args.out)


if __name__ == "__main__":
    main()
