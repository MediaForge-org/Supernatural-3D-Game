#!/usr/bin/env python3
"""Summarize the WORLD_700 GeoPackage (read-only; sqlite3 stdlib only).
Usage: gpkg_summary.py <path.gpkg> [table ...]  -> prints feature_id | name | class | state | properties_json (shortened)."""
import sqlite3, sys, json
con = sqlite3.connect("file:%s?mode=ro" % sys.argv[1], uri=True)
tables = sys.argv[2:] or [r[0] for r in con.execute("select table_name from gpkg_contents")]
for t in tables:
    cols = [r[1] for r in con.execute('pragma table_info("%s")' % t)]
    if "feature_id" not in cols:
        for r in con.execute('select * from "%s"' % t): print(t, r)
        continue
    print("##", t)
    for r in con.execute('select feature_id,name,class,state_id,data_status,properties_json from "%s"' % t):
        p = r[5]
        try: p = json.dumps(json.loads(p), ensure_ascii=False, separators=(",", ":"))
        except Exception: pass
        print("|", " | ".join(str(x) for x in r[:5]), "|", (p or "")[:420])
