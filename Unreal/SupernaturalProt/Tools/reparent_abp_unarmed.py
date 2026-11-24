"""0013 step S1: reparent the template anim blueprint ABP_Unarmed to the C++ base USpnAnimInstance (no visual change).

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file>
Its own variables and graphs stay untouched; the base only adds read-only Spn* variables (movement state interface).
Undo: restore the asset from git (git restore Unreal/SupernaturalProt/Content/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.uasset).
"""

import unreal

PATH = "/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"
BASE = "/Script/SupernaturalProt.SpnAnimInstance"


def main():
    bp = unreal.load_asset(PATH)
    base = unreal.load_class(None, BASE)
    if bp is None or base is None:
        unreal.log_error("SPN reparent: missing %s or %s" % (PATH, BASE))
        return
    def derives():
        return isinstance(unreal.get_default_object(bp.generated_class()), unreal.SpnAnimInstance)

    unreal.log("SPN reparent: %s derives from SpnAnimInstance before: %s" % (PATH, derives()))
    if derives():
        unreal.log("SPN reparent: already reparented, nothing to do")
        return
    unreal.BlueprintEditorLibrary.reparent_blueprint(bp, base)
    unreal.BlueprintEditorLibrary.compile_blueprint(bp)
    unreal.log("SPN reparent: derives after: %s" % derives())
    if not derives():
        unreal.log_error("SPN reparent: failed, not saved")
        return
    saved = unreal.EditorAssetLibrary.save_loaded_asset(bp, only_if_is_dirty=False)
    unreal.log("SPN reparent: saved=%s" % saved)


main()
