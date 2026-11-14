"""Wspólne funkcje skryptów budowy świata (uruchamiane w edytorze UE: Scripts/zw.cmd py <nazwa>)."""
import unreal

_errors = []


def log(msg):
    unreal.log("ZW " + msg)


def error(msg):
    """Błąd zgłaszany do Scripts/zw.ps1 (szuka znacznika ZW-BLAD w logu)."""
    _errors.append(msg)
    unreal.log_error("ZW-BLAD " + msg)


def errors():
    return list(_errors)


def load(path):
    asset = unreal.EditorAssetLibrary.load_asset(path)
    if asset is None:
        error("brak assetu " + path)
    return asset


def actors():
    return unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def spawn(cls, loc=(0, 0, 0), rot=(0, 0, 0), label=None):
    """Stawia aktora; loc w cm, rot jako (pitch, yaw, roll)."""
    a = actors().spawn_actor_from_class(cls, unreal.Vector(*loc), unreal.Rotator(roll=rot[2], pitch=rot[0], yaw=rot[1]))
    if a is None:
        error("nie udało się postawić " + str(cls))
        return None
    if label:
        a.set_actor_label(label)
    return a


def mesh(mesh_path, loc, scale=(1, 1, 1), rot=(0, 0, 0), material=None, label=None):
    a = spawn(unreal.StaticMeshActor, loc, rot, label)
    if a is None:
        return None
    comp = a.static_mesh_component
    sm = load(mesh_path)
    if sm:
        comp.set_static_mesh(sm)
    if material:
        mat = load(material)
        if mat:
            comp.set_material(0, mat)
    a.set_actor_scale3d(unreal.Vector(*scale))
    return a


def new_map(path):
    les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if unreal.EditorAssetLibrary.does_asset_exist(path):
        unreal.EditorAssetLibrary.delete_asset(path)
    if not les.new_level(path):
        error("nie udało się utworzyć mapy " + path)
        return False
    return True


def save_map():
    les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if not les.save_current_level():
        error("nie udało się zapisać mapy")
        return False
    return True
