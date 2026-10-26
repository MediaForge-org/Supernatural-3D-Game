"""Layout of the PLACEHOLDER movement test map /Game/SpnTest/Lvl_SpnMovementTest (Docs/Proposals/0011, M2-A / M2-A.1).

Pure Python, no Unreal import: it describes the map as data (boxes, cylinders, spheres, doors, one guard volume),
so the geometry can be checked on any machine (Tools/test_spn_movement_map_layout.py) and is spawned in the editor
by create_spn_movement_test_map.py. Deterministic: the same code always gives the same layout.

This map is a development and test environment. It is NOT a geographic excerpt of the later open world and implies
no decision about terrain, roads, settlements or buildings of the real game. All sizes are throw-away test values.

Areas (work order M2-A.1, section 11): A flat ground (plaza, speed lane), B slopes (ramps, hills), C stairs, D obstacles and
walls, E uneven ground (terraces, rocks, surface patches), F camera tests (alley, pillar field), G buildings and interiors
(hut, house, warehouse), H boundary and reset points (four walls, play-area guard, reset points). Every raised platform, ramp
and staircase has rails or side walls: the map has no open drop edges.

Units: centimetres (Unreal). X east, Y north, Z up. The ground surface is Z = 0. The map is 300 x 300 m.
Rotation is (pitch, yaw, roll) in degrees as in Unreal; positive pitch raises the +X end.
"""

import json
import math

HALF = 15000.0  # half edge of the playable square: 300 m x 300 m
WALL_THICKNESS = 1000.0
WALL_HEIGHT = 2000.0

# Obstacle heights, cm: 20/25/30/40/45/50/75/100/150 from the M2-A.1 work order (section 13) plus 60/80/120 from M2-A.
# The character's step height is 25 cm and its jump apex 45 cm (defaults of FSpnMovementSettings): 20 is a step, 25 the
# step limit, 30 and 40 are jump candidates, 45 is the jump apex, everything above is blocked until vault/mantle exist.
OBSTACLE_HEIGHTS = (20, 25, 30, 40, 45, 50, 60, 75, 80, 100, 120, 150)
MAX_STEP_HEIGHT = 25.0  # cm, default of FSpnMovementSettings::MaxStepHeight (the layout test compares the C++ default)
JUMP_HEIGHT = 45.0      # cm, default of FSpnMovementSettings::JumpHeight
RAMP_ANGLES = (5, 10, 15, 20, 30, 40, 50)  # degrees; the default walkable floor angle is ~44.8
# (rise, run) per staircase, cm. 25 is the step limit, 30/40/50 are above it on purpose.
STAIRCASES = ((15, 35), (18, 30), (22, 28), (25, 27), (30, 25), (40, 25), (50, 25))
STAIR_STEPS = 10
OBSTACLE_SPACING = 600.0     # between neighbouring obstacles along Y
THIN_X, THIN_DEPTH = 3000.0, 40.0     # row 1: thin walls (step-up / jump / later vault)
THICK_X, THICK_DEPTH = 5000.0, 200.0  # row 2: thick blocks (later mantle)
RAMP_START_X = -9000.0
RAMP_LENGTH = 1500.0
RAMP_SPACING = 700.0
STAIR_START_X = -9000.0
STAIR_FIRST_Y = 4000.0
STAIR_SPACING = 500.0
RAIL_HEIGHT = 120.0   # above every walkable surface with a drop; more than the jump apex plus the step height
RAIL_THICKNESS = 20.0

PLAYER_START = (0.0, 0.0, 100.0)

# Speed lane: a free strip along +Y for the speed tests (nothing but flat markers stands in it).
LANE_X = -12500.0
LANE_START_Y = -12000.0
LANE_MARKER_SPACING = 1000.0
LANE_MARKERS = 11

# Capsule of the character (cm): used by the layout test to keep reset points free.
CAPSULE_RADIUS = 42.0
CAPSULE_HALF_HEIGHT = 96.0

# Bounding rectangles (x0, x1, y0, y1) of the zones; the layout test checks they do not overlap.
ZONES = {
    "plaza": (-1500.0, 1500.0, -1500.0, 1500.0),
    "speedlane": (-14000.0, -11000.0, -13000.0, 13000.0),
    "obstacles": (2500.0, 5800.0, -3700.0, 3700.0),
    "ramps": (-9500.0, -6000.0, -2800.0, 2800.0),
    "stairs": (-9500.0, -8300.0, 3700.0, 7300.0),
    "pillars": (-5500.0, -2500.0, -6500.0, -2500.0),
    "hills": (6000.0, 13500.0, -9800.0, -1200.0),
    "terraces": (7000.0, 11000.0, 2000.0, 6000.0),
    "rocks": (6500.0, 11500.0, 7000.0, 12000.0),
    "surfaces": (-6000.0, 2000.0, 9000.0, 14000.0),
    "alley": (-1500.0, 600.0, 3500.0, 7500.0),
    "hut": (-5000.0, -3200.0, -9800.0, -8200.0),
    "house": (-1000.0, 1000.0, -10200.0, -8000.0),
    "warehouse": (2200.0, 5600.0, -12800.0, -10000.0),
}

# Work-order areas A..H and the zones that belong to them.
AREAS = {
    "A_flat": ("plaza", "speedlane"),
    "B_slopes": ("ramps", "hills"),
    "C_stairs": ("stairs",),
    "D_obstacles": ("obstacles",),
    "E_uneven": ("terraces", "rocks", "surfaces"),
    "F_camera": ("alley", "pillars"),
    "G_buildings": ("hut", "house", "warehouse"),
    "H_boundary": ("boundary",),
}

# Reset points (name, x, y): one on open ground next to every area. The play-area guard puts a pawn back at the nearest one.
RESET_POINTS = (
    ("plaza", 0.0, 0.0),
    ("speedlane", LANE_X, LANE_START_Y),
    ("obstacles", 1800.0, 0.0),
    ("ramps", -9900.0, 0.0),
    ("stairs", -9900.0, 5500.0),
    ("hills", 6500.0, 0.0),
    ("terraces", 6300.0, 4000.0),
    ("rocks", 6000.0, 9500.0),
    ("surfaces", -2000.0, 8300.0),
    ("alley", 0.0, 3000.0),
    ("pillars", -4000.0, -2000.0),
    ("hut", -4000.0, -7500.0),
    ("house", 0.0, -7500.0),
    ("warehouse", 3500.0, -9000.0),
)

MATERIAL_PATHS = {
    "grid_gray": "/Game/LevelPrototyping/Materials/MI_PrototypeGrid_Gray.MI_PrototypeGrid_Gray",
    "grid_gray_02": "/Game/LevelPrototyping/Materials/MI_PrototypeGrid_Gray_02.MI_PrototypeGrid_Gray_02",
    "grid_round": "/Game/LevelPrototyping/Materials/MI_PrototypeGrid_Gray_Round.MI_PrototypeGrid_Gray_Round",
    "grid_topdark": "/Game/LevelPrototyping/Materials/MI_PrototypeGrid_TopDark.MI_PrototypeGrid_TopDark",
    "colorway": "/Game/LevelPrototyping/Materials/MI_DefaultColorway.MI_DefaultColorway",
}

SHAPE_PATHS = {
    "cube": "/Engine/BasicShapes/Cube.Cube",
    "cylinder": "/Engine/BasicShapes/Cylinder.Cylinder",
    "sphere": "/Engine/BasicShapes/Sphere.Sphere",
}


class _Lcg:
    """Small deterministic generator, independent of the Python version."""

    def __init__(self, seed):
        self.state = seed & 0xFFFFFFFF

    def next(self):
        self.state = (self.state * 1664525 + 1013904223) & 0xFFFFFFFF
        return self.state / 4294967296.0

    def uniform(self, low, high):
        return low + (high - low) * self.next()


class Layout:
    def __init__(self):
        self.prims = []
        self.doors = []
        self.openings = []

    def add(self, label, shape, center, size, zone, rot=(0.0, 0.0, 0.0), material="grid_gray"):
        """size = full extents in cm for cube; (diameter, diameter, height) for cylinder; diameter x3 for sphere."""
        self.prims.append({
            "label": label,
            "shape": shape,
            "center": [round(float(v), 3) for v in center],
            "size": [round(float(v), 3) for v in size],
            "rot": [round(float(v), 3) for v in rot],
            "material": material,
            "zone": zone,
        })

    def box_on_ground(self, label, x, y, width_x, width_y, height, zone, material="grid_gray"):
        self.add(label, "cube", (x, y, height / 2.0), (width_x, width_y, height), zone, material=material)

    def wall(self, label, zone, axis, fixed, a0, a1, z0, height, thickness, openings=(), material="grid_gray_02"):
        """A wall in the plane axis=fixed ('x': plane x = fixed, running along Y; 'y': running along X) from a0 to a1.

        openings: (o0, o1, zbottom, ztop) in the running coordinate and height; segments are built around them.
        """
        cuts = sorted(openings)
        cursor = a0
        index = 0
        pieces = []  # (c0, c1, zb, zt)
        for o0, o1, zb, zt in cuts:
            self.openings.append({
                "wall": label, "zone": zone, "axis": axis, "fixed": fixed, "from": o0, "to": o1,
                "kind": "doorway" if zb <= z0 + 0.5 else "window",
                "width": o1 - o0, "height": zt - zb, "sill": zb - z0,
            })
            if o0 > cursor:
                pieces.append((cursor, o0, z0, z0 + height))
            if zb > z0:
                pieces.append((o0, o1, z0, zb))
            if zt < z0 + height:
                pieces.append((o0, o1, zt, z0 + height))
            cursor = o1
        if cursor < a1:
            pieces.append((cursor, a1, z0, z0 + height))
        for c0, c1, zb, zt in pieces:
            index += 1
            run_centre = (c0 + c1) / 2.0
            z_centre = (zb + zt) / 2.0
            if axis == "x":
                self.add("%s_%02d" % (label, index), "cube", (fixed, run_centre, z_centre),
                         (thickness, c1 - c0, zt - zb), zone, material=material)
            else:
                self.add("%s_%02d" % (label, index), "cube", (run_centre, fixed, z_centre),
                         (c1 - c0, thickness, zt - zb), zone, material=material)

    def door(self, stable_id, x, y_centre, zone):
        """ASpnTestDoor in a wall plane x = const: hinge at the +Y edge of a 100 cm panel (as Lvl_SpnDoorTest)."""
        self.doors.append({"id": stable_id, "hinge": [x, y_centre + 50.0, 0.0], "yaw": -90.0, "zone": zone})

    def rail_box(self, label, zone, center, size, rot=(0.0, 0.0, 0.0)):
        self.add(label, "cube", center, size, zone, rot=rot, material="grid_topdark")

    def ramp(self, label, zone, x_start, y, length, width, thickness, angle, landing_length, material="grid_gray", rails=True):
        """Slab whose top face starts at the ground at x_start and rises towards +X, a solid landing at the top and rails on
        both sides of the slope and the landing and across the landing's far end (no open drop edge)."""
        a = math.radians(angle)
        half = length / 2.0
        centre = (
            x_start + half * math.cos(a) + thickness / 2.0 * math.sin(a),
            y,
            half * math.sin(a) - thickness / 2.0 * math.cos(a),
        )
        self.add(label, "cube", centre, (length, width, thickness), zone, rot=(angle, 0.0, 0.0), material=material)
        top = length * math.sin(a)
        end_x = x_start + length * math.cos(a)
        if rails:
            side = width / 2.0 - RAIL_THICKNESS / 2.0
            for tag, sign in (("W", -1.0), ("E", 1.0)):
                # Rail on the slope surface: same pitch, RAIL_HEIGHT above the surface along the slab normal.
                centre_r = (
                    x_start + half * math.cos(a) - RAIL_HEIGHT / 2.0 * math.sin(a),
                    y + sign * side,
                    half * math.sin(a) + RAIL_HEIGHT / 2.0 * math.cos(a),
                )
                self.rail_box("%s_Rail_%s" % (label, tag), zone, centre_r, (length, RAIL_THICKNESS, RAIL_HEIGHT), rot=(angle, 0.0, 0.0))
        if landing_length <= 0.0:
            return
        self.box_on_ground(label + "_Landing", end_x + landing_length / 2.0, y, landing_length, width, top, zone, material)
        if rails:
            self._landing_rails(label + "_Landing", zone, end_x, y, landing_length, width, top)

    def _landing_rails(self, label, zone, x_near, y, length, width, top):
        """Rails along both sides and across the far end of a platform whose near edge (x_near) is open for access."""
        side = width / 2.0 - RAIL_THICKNESS / 2.0
        for tag, sign in (("RailW", -1.0), ("RailE", 1.0)):
            self.rail_box("%s_%s" % (label, tag), zone, (x_near + length / 2.0, y + sign * side, top + RAIL_HEIGHT / 2.0),
                          (length, RAIL_THICKNESS, RAIL_HEIGHT))
        self.rail_box(label + "_EndRail", zone, (x_near + length - RAIL_THICKNESS / 2.0, y, top + RAIL_HEIGHT / 2.0),
                      (RAIL_THICKNESS, width, RAIL_HEIGHT))

    def stairs(self, label, zone, x_start, y, width, rise, run, steps, landing_length, material="grid_gray"):
        for k in range(1, steps + 1):
            self.box_on_ground("%s_Step_%02d" % (label, k), x_start + (k - 0.5) * run, y, run, width, k * rise, zone, material)
        top = steps * rise
        self.box_on_ground(label + "_Landing", x_start + steps * run + landing_length / 2.0, y, landing_length, width, top, zone, material)
        # Side walls (stringers) along the whole flight and the landing, RAIL_HEIGHT above the top; outside the clear width.
        total = steps * run + landing_length
        for tag, sign in (("W", -1.0), ("E", 1.0)):
            self.box_on_ground("%s_Side_%s" % (label, tag), x_start + total / 2.0, y + sign * (width / 2.0 + RAIL_THICKNESS / 2.0),
                               total, RAIL_THICKNESS, top + RAIL_HEIGHT, zone, "grid_topdark")
        self.rail_box(label + "_EndRail", zone, (x_start + total - RAIL_THICKNESS / 2.0, y, top + RAIL_HEIGHT / 2.0), (RAIL_THICKNESS, width, RAIL_HEIGHT))


def _build_base(layout):
    layout.add("SPN_MV_Floor", "cube", (0.0, 0.0, -50.0), (2 * HALF, 2 * HALF, 100.0), "base")
    # Four containment walls. The inner faces stand exactly on the +-HALF line; they reach below the floor line.
    centre = HALF + WALL_THICKNESS / 2.0
    span = 2 * HALF + 2 * WALL_THICKNESS
    z_centre = (WALL_HEIGHT - 200.0) / 2.0
    height = WALL_HEIGHT + 200.0
    layout.add("SPN_MV_Boundary_North", "cube", (0.0, centre, z_centre), (span, WALL_THICKNESS, height), "boundary", material="grid_topdark")
    layout.add("SPN_MV_Boundary_South", "cube", (0.0, -centre, z_centre), (span, WALL_THICKNESS, height), "boundary", material="grid_topdark")
    layout.add("SPN_MV_Boundary_East", "cube", (centre, 0.0, z_centre), (WALL_THICKNESS, span, height), "boundary", material="grid_topdark")
    layout.add("SPN_MV_Boundary_West", "cube", (-centre, 0.0, z_centre), (WALL_THICKNESS, span, height), "boundary", material="grid_topdark")


def obstacle_y(index):
    """Y of the obstacle with this index in OBSTACLE_HEIGHTS (the Unreal tests use the same rule)."""
    return (index - (len(OBSTACLE_HEIGHTS) - 1) / 2.0) * OBSTACLE_SPACING


def ramp_y(index):
    return (index - (len(RAMP_ANGLES) - 1) / 2.0) * RAMP_SPACING


def stair_y(index):
    return STAIR_FIRST_Y + index * STAIR_SPACING


def _build_obstacles(layout):
    # Row 1: thin walls (40 cm deep) for step-up and vaulting. Row 2: thick blocks (200 cm deep) for mantling.
    for x, depth, name in ((THIN_X, THIN_DEPTH, "Thin"), (THICK_X, THICK_DEPTH, "Thick")):
        for i, height in enumerate(OBSTACLE_HEIGHTS):
            y = obstacle_y(i)
            layout.box_on_ground("SPN_MV_Obstacle_%s_%03d" % (name, height), x, y, depth, 500.0, float(height), "obstacles")


def _build_ramps(layout):
    for i, angle in enumerate(RAMP_ANGLES):
        y = ramp_y(i)
        layout.ramp("SPN_MV_Ramp_%02d" % angle, "ramps", RAMP_START_X, y, RAMP_LENGTH, 400.0, 20.0, float(angle), 400.0)


def _build_stairs(layout):
    for i, (rise, run) in enumerate(STAIRCASES):
        layout.stairs("SPN_MV_Stairs_R%02d" % rise, "stairs", STAIR_START_X, stair_y(i), 300.0, float(rise), float(run), STAIR_STEPS, 300.0)


def _build_hills(layout):
    # Half-buried spheres: (sphere radius, cap height above the ground, x, y). Slope at the rim = acos((R - h) / R).
    hills = ((4000.0, 300.0, 7600.0, -4100.0), (6000.0, 500.0, 10000.0, -7400.0), (3000.0, 600.0, 11500.0, -3000.0))
    for i, (radius, cap, x, y) in enumerate(hills, 1):
        layout.add("SPN_MV_Hill_%d" % i, "sphere", (x, y, cap - radius), (2 * radius, 2 * radius, 2 * radius), "hills", material="grid_round")


def _build_terraces(layout):
    # Concentric cylinders, each 20 cm higher than the one outside it: a terraced hill of 20 cm steps.
    for tier in range(6):
        radius = 1800.0 - 300.0 * tier
        top = 20.0 * (tier + 1)
        layout.add("SPN_MV_Terrace_%d" % (tier + 1), "cylinder", (9000.0, 4000.0, top / 2.0), (2 * radius, 2 * radius, top), "terraces", material="grid_round")


def _build_rocks(layout):
    # Uneven ground: 80 low tilted boxes, half buried, every edge below the step height (top at most about 24 cm), deterministic.
    rng = _Lcg(20261025)
    for i in range(80):
        sx = rng.uniform(60.0, 180.0)
        sy = rng.uniform(60.0, 180.0)
        sz = rng.uniform(8.0, 16.0)
        x = rng.uniform(6700.0, 11300.0)
        y = rng.uniform(7200.0, 11800.0)
        rot = (rng.uniform(-3.0, 3.0), rng.uniform(0.0, 360.0), rng.uniform(-3.0, 3.0))
        layout.add("SPN_MV_Rock_%03d" % i, "cube", (x, y, sz / 2.0 - 2.0), (sx, sy, sz), "rocks", rot=rot, material="grid_gray_02")


def _build_surfaces(layout):
    # Visual surface patches only: the project has no physical materials yet, so footsteps/friction cannot differ.
    keys = ("grid_gray", "grid_gray_02", "grid_round", "grid_topdark", "colorway")
    for i, key in enumerate(keys):
        layout.add("SPN_MV_Surface_%d_%s" % (i + 1, key), "cube", (-5000.0 + i * 1400.0, 11500.0, 1.0), (1300.0, 4000.0, 2.0), "surfaces", material=key)


def _build_alley(layout):
    # Camera-collision alley: 150 cm wide between the inner wall faces, walls 300 cm high, 3000 cm long along Y,
    # with a low section (ceiling underside at 230 cm) over the middle 1000 cm.
    layout.wall("SPN_MV_Alley_West", "alley", "x", -85.0, 4000.0, 7000.0, 0.0, 300.0, 20.0)
    layout.wall("SPN_MV_Alley_East", "alley", "x", 85.0, 4000.0, 7000.0, 0.0, 300.0, 20.0)
    layout.add("SPN_MV_Alley_Ceiling", "cube", (0.0, 5500.0, 240.0), (190.0, 1000.0, 20.0), "alley", material="grid_gray_02")


def _build_hut(layout):
    x0, x1, y0, y1 = -4800.0, -4000.0, -9500.0, -8900.0  # 800 x 600 outer
    h = 280.0
    t = 20.0
    z = "hut"
    layout.add("SPN_MV_Hut_Floor", "cube", (-4400.0, -9200.0, 2.5), (x1 - x0, y1 - y0, 5.0), z)  # 5 cm raised floor
    layout.wall("SPN_MV_Hut_West", z, "x", x0, y0, y1, 5.0, h, t, openings=[(-9250.0, -9140.0, 5.0, 225.0)])
    layout.wall("SPN_MV_Hut_East", z, "x", x1, y0, y1, 5.0, h, t)
    layout.wall("SPN_MV_Hut_South", z, "y", y0, x0, x1, 5.0, h, t)
    layout.wall("SPN_MV_Hut_North", z, "y", y1, x0, x1, 5.0, h, t, openings=[(-4500.0, -4380.0, 95.0, 215.0)])  # window
    layout.add("SPN_MV_Hut_Roof", "cube", (-4400.0, -9200.0, 5.0 + h + 10.0), (x1 - x0 + 40.0, y1 - y0 + 40.0, 20.0), z)
    layout.door("mv_hut_door", x0, -9195.0, z)


def _build_house(layout):
    z = "house"
    x0, x1, y0, y1 = -700.0, 700.0, -9700.0, -8700.0  # 1400 x 1000 outer
    floor_h = 320.0  # first floor level
    t = 20.0
    h = 300.0
    gh = 295.0  # ground floor walls reach the underside of the first-floor slab (300 cm)
    # Ground floor shell: entrance door and a window in the west wall, windows south/north.
    layout.add("SPN_MV_House_Floor", "cube", (0.0, (y0 + y1) / 2.0, 2.5), (x1 - x0, y1 - y0, 5.0), z)
    layout.wall("SPN_MV_House_G_West", z, "x", x0, y0, y1, 5.0, gh, t, openings=[(-9255.0, -9145.0, 5.0, 225.0), (-8950.0, -8830.0, 95.0, 215.0)])
    layout.wall("SPN_MV_House_G_East", z, "x", x1, y0, y1, 5.0, gh, t)
    layout.wall("SPN_MV_House_G_South", z, "y", y0, x0, x1, 5.0, gh, t, openings=[(300.0, 420.0, 95.0, 215.0)])
    layout.wall("SPN_MV_House_G_North", z, "y", y1, x0, x1, 5.0, gh, t, openings=[(-300.0, -180.0, 95.0, 215.0)])
    # Interior ground floor: x = -100 divides hall and rooms (doorway), y = -9200 divides the rooms (doorway).
    layout.wall("SPN_MV_House_G_Divider", z, "x", -100.0, y0, y1, 5.0, gh, t, openings=[(-9300.0, -9190.0, 5.0, 225.0)])
    layout.wall("SPN_MV_House_G_RoomWall", z, "y", -9200.0, -100.0, x1, 5.0, gh, t, openings=[(300.0, 410.0, 5.0, 225.0)])
    # Interior stairs along +Y in the hall, west side: 17 steps to the first floor.
    steps = 17
    rise = (floor_h - 5.0) / steps
    run = 28.0
    for i in range(1, steps + 1):
        layout.add("SPN_MV_House_Stair_%02d" % i, "cube", (-625.0, y0 + 20.0 + (i - 0.5) * run, 5.0 + i * rise / 2.0), (130.0, run, i * rise), z)
    hole_y1 = y0 + 20.0 + steps * run  # top of the stairs
    # First floor slab (320 cm level, 20 cm thick) with a stairwell opening above the stairs.
    slab_top = floor_h
    slab_c = slab_top - 10.0
    layout.add("SPN_MV_House_Slab_West", "cube", (x0 + 5.0, (y0 + y1) / 2.0, slab_c), (10.0, y1 - y0, 20.0), z)
    layout.add("SPN_MV_House_Slab_Main", "cube", ((-560.0 + x1) / 2.0, (y0 + y1) / 2.0, slab_c), (x1 + 560.0, y1 - y0, 20.0), z)
    layout.add("SPN_MV_House_Slab_North", "cube", (-625.0, (hole_y1 + y1) / 2.0, slab_c), (130.0, y1 - hole_y1, 20.0), z)
    # Rail along the open east edge of the stairwell (the north end is where the stairs arrive, the south end is the wall).
    layout.rail_box("SPN_MV_House_StairRail", z, (-555.0, (y0 + hole_y1) / 2.0, slab_top + 50.0), (10.0, hole_y1 - y0, 100.0))
    # First floor shell and rooms.
    f0 = floor_h
    layout.wall("SPN_MV_House_F_West", z, "x", x0, y0, y1, f0, h, t, openings=[(-9100.0, -8980.0, f0 + 90.0, f0 + 210.0)])
    layout.wall("SPN_MV_House_F_East", z, "x", x1, y0, y1, f0, h, t)
    layout.wall("SPN_MV_House_F_South", z, "y", y0, x0, x1, f0, h, t)
    layout.wall("SPN_MV_House_F_North", z, "y", y1, x0, x1, f0, h, t)
    layout.wall("SPN_MV_House_F_Divider", z, "x", -100.0, y0, y1, f0, h, t, openings=[(-9150.0, -9040.0, f0, f0 + 220.0)])
    layout.add("SPN_MV_House_Roof", "cube", (0.0, (y0 + y1) / 2.0, f0 + h + 10.0), (x1 - x0 + 40.0, y1 - y0 + 40.0, 20.0), z)
    layout.door("mv_house_door", x0, -9200.0, z)


def _build_warehouse(layout):
    z = "warehouse"
    x0, x1, y0, y1 = 3000.0, 5000.0, -12400.0, -11200.0  # 2000 x 1200 outer
    h = 600.0
    t = 30.0
    layout.add("SPN_MV_Warehouse_Floor", "cube", (4000.0, -11800.0, 50.0), (x1 - x0, y1 - y0, 100.0), z)  # 100 cm loading dock height
    layout.wall("SPN_MV_Warehouse_West", z, "x", x0, y0, y1, 100.0, h, t, openings=[(-12000.0, -11700.0, 100.0, 500.0)])
    layout.wall("SPN_MV_Warehouse_East", z, "x", x1, y0, y1, 100.0, h, t)
    layout.wall("SPN_MV_Warehouse_South", z, "y", y0, x0, x1, 100.0, h, t)
    layout.wall("SPN_MV_Warehouse_North", z, "y", y1, x0, x1, 100.0, h, t, openings=[(3400.0, 3600.0, 190.0, 310.0), (4400.0, 4510.0, 190.0, 310.0)])  # windows with a sill; no opening to the 1 m drop outside
    layout.add("SPN_MV_Warehouse_Pillar_1", "cube", (3700.0, -11800.0, 100.0 + h / 2.0), (60.0, 60.0, h), z)
    layout.add("SPN_MV_Warehouse_Pillar_2", "cube", (4300.0, -11800.0, 100.0 + h / 2.0), (60.0, 60.0, h), z)
    layout.add("SPN_MV_Warehouse_Roof", "cube", (4000.0, -11800.0, 100.0 + h + 10.0), (x1 - x0 + 60.0, y1 - y0 + 60.0, 20.0), z)
    # Loading ramp on the west side: 100 cm rise over a 700 cm slope (about 8 degrees), ending at the dock edge.
    angle = math.degrees(math.asin(100.0 / 700.0))
    layout.ramp("SPN_MV_Warehouse_DockRamp", z, x0 - 700.0 * math.cos(math.radians(angle)), -11850.0, 700.0, 300.0, 20.0, angle, 0.0, rails=True)


def _build_pillars(layout):
    # Camera test: a field of 5 x 5 pillars (80 cm across, 4 m high, 4 m apart) for fast direction changes between obstacles.
    for ix in range(5):
        for iy in range(5):
            layout.add("SPN_MV_Pillar_%d%d" % (ix, iy), "cylinder", (-5300.0 + ix * 400.0, -6300.0 + iy * 400.0, 200.0), (80.0, 80.0, 400.0), "pillars", material="grid_round")


def _build_speedlane(layout):
    # Flat distance markers (1 cm high, 10 m apart) for the speed tests; the lane is otherwise empty and level.
    for k in range(LANE_MARKERS):
        layout.add("SPN_MV_Lane_Marker_%03d" % (k * 10), "cube", (LANE_X, LANE_START_Y + k * LANE_MARKER_SPACING, 0.5), (600.0, 20.0, 1.0), "speedlane")


def build_layout():
    layout = Layout()
    _build_base(layout)
    _build_obstacles(layout)
    _build_ramps(layout)
    _build_stairs(layout)
    _build_hills(layout)
    _build_terraces(layout)
    _build_rocks(layout)
    _build_surfaces(layout)
    _build_alley(layout)
    _build_hut(layout)
    _build_house(layout)
    _build_warehouse(layout)
    _build_pillars(layout)
    _build_speedlane(layout)
    return {
        "map": "/Game/SpnTest/Lvl_SpnMovementTest",
        "half_extent_cm": HALF,
        "player_start": list(PLAYER_START),
        "guard": {"center": [0.0, 0.0, 1000.0], "extent": [HALF, HALF, 5000.0], "safe_location": list(PLAYER_START),
                  "reset_points": [[x, y, PLAYER_START[2]] for _, x, y in RESET_POINTS]},
        "reset_points": [{"name": name, "pos": [x, y, PLAYER_START[2]]} for name, x, y in RESET_POINTS],
        "areas": {name: list(zones) for name, zones in AREAS.items()},
        "prims": layout.prims,
        "doors": layout.doors,
        "openings": layout.openings,
    }


def to_json(layout_data):
    return json.dumps(layout_data, indent=1, sort_keys=True)


if __name__ == "__main__":
    import sys

    data = build_layout()
    text = to_json(data)
    if len(sys.argv) > 1:
        with open(sys.argv[1], "w", encoding="utf-8") as handle:
            handle.write(text + "\n")
    print("%d primitives, %d doors" % (len(data["prims"]), len(data["doors"])))
