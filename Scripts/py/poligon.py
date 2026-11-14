"""Mapa testowa E0: /Game/Zwiadowcy/Mapy/L_Poligon.

Płaska podłoga 200x200 m, niebo z atmosferą, nisko stojące ciepłe słońce (złote popołudnie), mgła w dolinie,
kilka przeszkód do sprawdzenia ruchu, kucania i kamery. Uruchamianie: Scripts/zw.cmd py poligon
"""
import os
import sys

sys.path.append(os.path.dirname(os.path.abspath(__file__)))

import unreal
import zw_common as zw

MAP = "/Game/Zwiadowcy/Mapy/L_Poligon"
CUBE = "/Engine/BasicShapes/Cube"
CYLINDER = "/Engine/BasicShapes/Cylinder"
GRID = "/Game/LevelPrototyping/Materials/MI_PrototypeGrid_Gray"
GRID_DARK = "/Game/LevelPrototyping/Materials/MI_PrototypeGrid_TopDark"
RAMP = "/Game/LevelPrototyping/Meshes/SM_Ramp"


def light_and_sky():
    sun = zw.spawn(unreal.DirectionalLight, (0, 0, 1000), (-14, 135, 0), "Slonce")
    c = sun.get_component_by_class(unreal.DirectionalLightComponent)
    c.set_mobility(unreal.ComponentMobility.MOVABLE)
    c.set_editor_property("intensity", 9.0)
    c.set_editor_property("use_temperature", True)
    c.set_editor_property("temperature", 4300.0)
    c.set_editor_property("atmosphere_sun_light", True)
    c.set_editor_property("dynamic_shadow_distance_movable_light", 12000.0)

    zw.spawn(unreal.SkyAtmosphere, (0, 0, 0), label="Atmosfera")

    sky = zw.spawn(unreal.SkyLight, (0, 0, 500), label="SwiatloNieba")
    sc = sky.get_component_by_class(unreal.SkyLightComponent)
    sc.set_mobility(unreal.ComponentMobility.MOVABLE)
    sc.set_editor_property("real_time_capture", True)
    sc.set_editor_property("intensity", 1.0)

    fog = zw.spawn(unreal.ExponentialHeightFog, (0, 0, -200), label="Mgla")
    fc = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
    fc.set_editor_property("fog_density", 0.025)
    fc.set_editor_property("fog_height_falloff", 0.25)
    fc.set_editor_property("enable_volumetric_fog", True)
    fc.set_editor_property("volumetric_fog_scattering_distribution", 0.6)

    ppv = zw.spawn(unreal.PostProcessVolume, (0, 0, 0), label="Postprocess")
    ppv.set_editor_property("unbound", True)
    s = ppv.get_editor_property("settings")
    for name, value in [
        ("vignette_intensity", 0.45),
        ("film_grain_intensity", 0.12),
        ("bloom_intensity", 0.5),
        ("auto_exposure_bias", 0.3),
        ("color_saturation", unreal.Vector4(0.92, 0.92, 0.92, 1.0)),
        ("color_gain_highlights", unreal.Vector4(1.04, 1.0, 0.94, 1.0)),
    ]:
        s.set_editor_property("override_" + name, True)
        s.set_editor_property(name, value)
    ppv.set_editor_property("settings", s)


def ground_and_props():
    # podłoga 200x200 m, górna powierzchnia na z=0
    zw.mesh(CUBE, (0, 0, -50), (2000, 2000, 1), material=GRID, label="Podloga")

    # mur do sprawdzania kamery przy ścianie i niska przeszkoda do kucania
    zw.mesh(CUBE, (1200, 0, 200), (1, 12, 4), material=GRID_DARK, label="Mur")
    zw.mesh(CUBE, (800, -600, 150), (4, 4, 0.1), material=GRID_DARK, label="Daszek")
    for i in range(4):
        zw.mesh(CUBE, (800, -600 + (i - 1.5) * 180, 70), (0.2, 0.2, 1.4), material=GRID_DARK, label="Slupek%d" % i)

    # rampa i schody
    zw.mesh(RAMP, (0, 900, 0), (2, 2, 2), material=GRID, label="Rampa")
    for i in range(6):
        zw.mesh(CUBE, (-800 - i * 60, 600, 10 + i * 20), (0.6, 3, 0.2 + i * 0.4), material=GRID, label="Schod%d" % i)

    # kolumny jak w ruinach - do oceny skali i światła
    for i in range(5):
        zw.mesh(CYLINDER, (-1500, -1000 + i * 500, 300), (1.2, 1.2, 6), material=GRID_DARK, label="Kolumna%d" % i)

    zw.spawn(unreal.PlayerStart, (0, 0, 120), (0, 0, 0), "Start")


def main():
    if not zw.new_map(MAP):
        return
    light_and_sky()
    ground_and_props()
    if zw.save_map():
        zw.log("zapisano " + MAP)


main()
