#!/usr/bin/env python3
"""Topic inventory of the planning master (read-only). Python stdlib only.
Usage: SPN_MASTER=<cumulative master dir> inventory.py <outdir>
Writes topic_files.tsv: path, size, tier, per-topic hit counts (text files < 3 MB)."""
import os, re, sys, csv
root = os.environ.get("SPN_MASTER", "/mnt/Festplatte/SPN_FULL_MASTER_2026-09-27/SPN_FULL_CUMULATIVE_MASTER_2026-09-27")
out = sys.argv[1]
os.makedirs(out, exist_ok=True)
TOPICS = {
 "map": r"world map|master map|heightmap|GIS\b|landscape|geograph|coastline|terrain|km²|km2|WORLD_1500|WORLD_700",
 "metro": r"metropolis|downtown|skyline|district",
 "cities": r"glenhaven|ashford|settlement|small town|suburb",
 "nature": r"forest|mountain|wilderness|river|lake alder|reservoir|valley",
 "roads": r"railway|rail\b|highway|road network|subway|metro line|tram|airport|bridge",
 "underground": r"underground|subterranean|cavern|cave city|bunker",
 "society": r"free housing|no government|tax|economy|NPC society|U2[4-9]|U3[0-6]",
 "vehicle": r"vehicle|drivetrain|engine|gearbox|clutch|car class|truck|motorcycle",
 "racing": r"racing|motorsport|race track|drift|rally|lap time",
 "worldgen": r"world partition|PCG|nanite|HLOD|data layer|level streaming|streaming",
}
TOP = {k: re.compile(v, re.I) for k, v in TOPICS.items()}
EXT = {".md", ".txt", ".csv", ".json", ".yml", ".yaml", ".tsv"}
rows = []
for dp, dn, fn in os.walk(root):
    for f in fn:
        p = os.path.join(dp, f); rel = os.path.relpath(p, root)
        try: sz = os.path.getsize(p)
        except OSError: continue
        ext = os.path.splitext(f)[1].lower()
        counts = {k: 0 for k in TOPICS}
        if ext in EXT and sz < 3_000_000 and "/extracted_" not in rel:
            try: t = open(p, encoding="utf-8", errors="ignore").read()
            except OSError: t = ""
            for k, rx in TOP.items(): counts[k] = len(rx.findall(t))
        tier = rel.split("/")[0]
        rows.append([rel, sz, tier] + [counts[k] for k in TOPICS])
with open(os.path.join(out, "topic_files.tsv"), "w", newline="") as fh:
    w = csv.writer(fh, delimiter="\t"); w.writerow(["path", "size", "tier"] + list(TOPICS)); w.writerows(rows)
print(len(rows), "files")
