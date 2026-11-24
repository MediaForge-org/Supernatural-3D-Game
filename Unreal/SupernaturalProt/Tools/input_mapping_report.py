"""READ-ONLY: lists the key mappings of the project's Enhanced Input mapping contexts (night build 2026-11-14, controller audit).

Run headless:  UnrealEditor-Cmd.exe <project> -run=pythonscript -script=<this file>
Writes Saved/SpnWorldGen/input_mappings.json. Saves nothing.
"""

import json
import os

import unreal

CONTEXTS = ["/Game/Input/IMC_Default", "/Game/Input/IMC_MouseLook"]
OUT = os.path.join(unreal.Paths.project_saved_dir(), "SpnWorldGen", "input_mappings.json")


def main():
    report = {}
    for path in CONTEXTS:
        imc = unreal.load_asset(path)
        if imc is None:
            report[path] = "missing"
            continue
        rows = []
        for m in imc.get_editor_property("default_key_mappings").get_editor_property("mappings"):
            action = m.get_editor_property("action")
            key = m.get_editor_property("key")
            mods = [type(x).__name__ for x in (m.get_editor_property("modifiers") or [])]
            trig = [type(x).__name__ for x in (m.get_editor_property("triggers") or [])]
            rows.append({"action": action.get_name() if action else None, "key": str(key.get_editor_property("key_name")), "modifiers": mods, "triggers": trig})
        report[path] = rows
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(report, f, indent=1)
    unreal.log("input_mapping_report -> " + OUT)


main()
