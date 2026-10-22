#!/usr/bin/env python3
"""Split reconstruction-pack chat transcripts into JSON message lists (read-only on master).
Usage: split_chats.py MASTER_DIR OUT_DIR [chat numbers...]  (default: all world-relevant chats)
Entry: [number, role, timestamp, text]. Python stdlib only."""
import glob, json, os, re, sys
DEFAULT = ["17","18","19","23","30","33","34","35","37","38","40","43","45","47","48","54"]
def main():
    master, out = sys.argv[1], sys.argv[2]
    nums = sys.argv[3:] or DEFAULT
    base = os.path.join(master, "99_RECONSTRUCTION_SOURCES/2026-10-14/SPN_RECONSTRUCTION_SOURCE_PACK_2026-10-14/QUELLEN/GESPRAECHE")
    os.makedirs(out, exist_ok=True)
    for n in nums:
        path = glob.glob(os.path.join(base, n.zfill(2) + "_*"))[0]
        text = open(path, encoding="utf8").read()
        parts = re.split(r"^## (\d+)\. (user|assistant) · (\S+)\n", text, flags=re.M)
        msgs = [(int(parts[i]), parts[i+1], parts[i+2], parts[i+3]) for i in range(1, len(parts), 4)]
        json.dump(msgs, open(os.path.join(out, f"msgs{n}.json"), "w"))
        u = sum(1 for m in msgs if m[1] == "user")
        print(n, os.path.basename(path)[:45], len(msgs), "msgs,", u, "user,", sum(len(m[3]) for m in msgs)//1000, "kB", msgs[0][2][:10], msgs[-1][2][:10])
main()
