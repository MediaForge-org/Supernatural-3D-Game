"""Run a Python file (with arguments) or a statement in a RUNNING Unreal Editor via the PythonScriptPlugin remote execution.

The editor must be started with remote execution enabled, e.g. on the command line (no config file change needed):
  -ini:EditorPerProjectUserSettings:[/Script/PythonScriptPlugin.PythonScriptUserSettings]:bRemoteExecution=True
Run this client with the engine's Python (it imports remote_execution.py from the engine plugin):
  <UE>/Engine/Binaries/ThirdParty/Python3/Win64/python.exe spn_remote_exec.py file <script.py> [args...]
  <UE>/Engine/Binaries/ThirdParty/Python3/Win64/python.exe spn_remote_exec.py exec "<python statement(s)>"
Set UE_ROOT if the engine is not at C:/Program Files/Unreal Engine/UE_5.8. Exit code 0 = success.
"""

import os
import sys
import time

UE_ROOT = os.environ.get("UE_ROOT", r"C:\Program Files\Unreal Engine\UE_5.8")
sys.path.insert(0, os.path.join(UE_ROOT, "Engine", "Plugins", "Experimental", "PythonScriptPlugin", "Content", "Python"))
import remote_execution as rex  # noqa: E402


def main(argv):
    mode, payload = argv[0], argv[1]
    if mode == "file":
        cmd = payload.replace("\\", "/")
        if len(argv) > 2:
            cmd = '"' + cmd + '" ' + " ".join('"' + a.replace("\\", "/") + '"' for a in argv[2:])
    else:
        cmd = payload
    remote = rex.RemoteExecution()
    remote.start()
    try:
        node = None
        for _ in range(60):
            if remote.remote_nodes:
                node = remote.remote_nodes[0]["node_id"]
                break
            time.sleep(0.5)
        if not node:
            print("no editor with remote execution found")
            return 2
        remote.open_command_connection(node)
        res = remote.run_command(cmd, unattended=True, exec_mode=rex.MODE_EXEC_FILE)
    finally:
        remote.stop()
    for o in res.get("output", []):
        print(f"[{o['type']}] {o['output']}".rstrip())
    print("RESULT:", res.get("success"), res.get("result"))
    return 0 if res.get("success") else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
