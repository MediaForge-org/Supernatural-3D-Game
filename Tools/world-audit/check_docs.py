#!/usr/bin/env python3
"""Checks for Docs/Game/World (read-only on master). Python stdlib only.
Usage: SPN_MASTER=<cumulative master> SPN_WORLD_DOCS=<Docs/Game/World> python3 check_docs.py <workdir with msgsNN.json>
Checks: (1) chat citations NN#m[E|A]: message exists, role matches suffix;
        (2) verbatim quotes „…“: normalized substring of chats/master text;
        (3) source keys (M-*, D1x-*, G7-*) used in the docs are defined in module 01;
        (4) decision IDs W-XX-nnn: defined once in module 20, every referenced ID exists."""
import glob, json, os, re, sys, unicodedata
work = sys.argv[1]; master = os.environ["SPN_MASTER"]; docs = os.environ["SPN_WORLD_DOCS"]
def norm(t):
    t = t.lower().replace("ß", "ss").replace("²", "2")
    t = unicodedata.normalize("NFKD", t)
    t = re.sub(r"[^a-z0-9]+", "", t)
    return t
msgs = {}
for f in glob.glob(os.path.join(work, "msgs*.json")):
    n = re.search(r"msgs(\d+)\.json", f).group(1)
    msgs[n] = {m[0]: m for m in json.load(open(f))}
files = sorted(glob.glob(os.path.join(docs, "*.md")))
GENERATED = ("25-",)
# 1 citations
bad_c = []; tot_c = 0
cite = re.compile(r"\b(\d{2})#(\d+)([EA])?\b")
for f in files:
    if os.path.basename(f).startswith(GENERATED): continue
    for ln, line in enumerate(open(f, encoding="utf8"), 1):
        for m in cite.finditer(line):
            tot_c += 1; n, k, r = m.group(1), int(m.group(2)), m.group(3)
            if n not in msgs or k not in msgs[n]: bad_c.append((os.path.basename(f), ln, m.group(0), "missing")); continue
            role = msgs[n][k][1]
            if r and ((r == "E") != (role == "user")): bad_c.append((os.path.basename(f), ln, m.group(0), "role " + role))
print("citations", tot_c, "problems", len(bad_c)); [print("  ", b) for b in bad_c[:60]]
# 2 quotes
corpus = []
for n in msgs:
    for m in msgs[n].values(): corpus.append(norm(m[3]))
for pat in ("01_CURRENT_CANONICAL_STATE/**/*", "99_DAILY_DELTAS/**/*", "99_CANON_AUDITS/**/*", "02_EMBEDDED_BASE_ARCHIVES/01_CURRENT_CANONICAL_STATE/*", "02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/reference/*", "02_EMBEDDED_BASE_ARCHIVES/02_PROMPT_LIBRARY_CURRENT/docs/planning-backlog/*", "_SOURCE_ARCHIVE/World Current/**/*.md", "_SOURCE_ARCHIVE/World Current/**/*.json", "_SOURCE_ARCHIVE/World History Loose Early/**/*.md", "02_WORLD_700/**/*.md", "02_WORLD_700/**/*.json"):
    for p in glob.glob(os.path.join(master, pat), recursive=True):
        if os.path.isfile(p) and p.endswith((".md", ".json", ".txt", ".csv")) and os.path.getsize(p) < 6_000_000:
            corpus.append(norm(open(p, encoding="utf8", errors="ignore").read()))
big = "\x00".join(corpus)
bad_q = []; tot_q = 0
for f in files:
    if os.path.basename(f).startswith(GENERATED): continue
    for ln, line in enumerate(open(f, encoding="utf8"), 1):
        for q in re.findall(r"„([^“]{8,300})“", line):
            nq = norm(q)
            if len(nq) < 8: continue
            tot_q += 1
            if nq not in big: bad_q.append((os.path.basename(f), ln, q[:90]))
print("quotes", tot_q, "not found", len(bad_q)); [print("  ", b) for b in bad_q[:80]]
# 3 source keys
m01 = open(os.path.join(docs, "01-Quellenlage-und-Methodik.md"), encoding="utf8").read()
defined = set(re.findall(r"`((?:M|D1[4-9]|G7|PE)-[A-Z0-9]+)`", m01))
used = set(); bad_k = []
key = re.compile(r"\b((?:M|D1[4-9]|G7|PE)-[A-Z][A-Z0-9]{1,6})\b")
for f in files:
    if os.path.basename(f).startswith(GENERATED): continue
    for ln, line in enumerate(open(f, encoding="utf8"), 1):
        for m in key.finditer(line):
            used.add(m.group(1))
            if m.group(1) not in defined and not m.group(1).startswith(("M-W", "M-D")) or (m.group(1) not in defined): bad_k.append((os.path.basename(f), ln, m.group(1)))
print("source keys defined", len(defined), "used", len(used), "undefined uses", len(bad_k)); [print("  ", b) for b in sorted(set(bad_k))[:40]]
# 4 decision ids
PFX = os.environ.get("SPN_ID_PREFIX", "W")  # W = World, E = Ermittlung
reg = open(os.path.join(docs, os.environ.get("SPN_REGISTER_FILE", "20-Entscheidungsregister.md")), encoding="utf8").read()  # NPCs: 26-Entscheidungsregister.md
ids = re.findall(r"^\| (" + PFX + r"-[A-Z]{2}-\d{3}) \|", reg, flags=re.M)
dups = sorted({i for i in ids if ids.count(i) > 1})
refd = set(); missing = []
for f in files:
    for ln, line in enumerate(open(f, encoding="utf8"), 1):
        for m in re.finditer(r"\b" + PFX + r"-[A-Z]{2}-\d{3}\b", line):
            refd.add(m.group(0))
            if m.group(0) not in ids: missing.append((os.path.basename(f), ln, m.group(0)))
print("register entries", len(ids), "duplicates", dups, "referenced", len(refd), "missing", len(missing)); [print("  ", b) for b in missing[:40]]
