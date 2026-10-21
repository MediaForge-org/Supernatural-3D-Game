#!/usr/bin/env python3
"""Split the chat transcripts of the reconstruction source pack into JSON message lists.

Read-only on the master. Usage: split_chats.py MASTER_DIR OUT_DIR
MASTER_DIR = .../SPN_FULL_CUMULATIVE_MASTER_2026-09-27
Writes OUT_DIR/msgs35.json, msgs37.json, msgs23.json, msgs28.json
Each entry: [number, role, timestamp, text].
"""
import glob, json, os, re, sys

CHATS = {"35": "35_Projektwissen", "37": "37_Tagesplan", "23": "23_SPN_Full_Master_ZIP", "28": "28_Adult_Content"}


def main():
    master, out = sys.argv[1], sys.argv[2]
    base = os.path.join(master, "99_RECONSTRUCTION_SOURCES/2026-10-14/SPN_RECONSTRUCTION_SOURCE_PACK_2026-10-14/QUELLEN/GESPRAECHE")
    os.makedirs(out, exist_ok=True)
    for tag, prefix in CHATS.items():
        path = glob.glob(os.path.join(base, prefix + "*"))[0]
        text = open(path, encoding="utf8").read()
        parts = re.split(r"^## (\d+)\. (user|assistant) · (\S+)\n", text, flags=re.M)
        msgs = [(int(parts[i]), parts[i + 1], parts[i + 2], parts[i + 3]) for i in range(1, len(parts), 4)]
        json.dump(msgs, open(os.path.join(out, f"msgs{tag}.json"), "w"))
        print(tag, len(msgs), "messages")


if __name__ == "__main__":
    main()
