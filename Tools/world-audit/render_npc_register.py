#!/usr/bin/env python3
"""Renders Docs/Game/NPCs/26-Entscheidungsregister.md from the ID tables of the NPC modules.
Usage: python3 render_npc_register.py [Docs/Game/NPCs]
Only the Python standard library. Rows are read, never written back to the modules.

A decision row in a module looks like:   | N-XX-nnn | statement | status | source |
A design rule row (group EN) looks like: | N-EN-nnn | rule | reference |
The generator checks: unique IDs, cell counts, known group codes, and that every referenced
N-XX-nnn in the modules exists. Exit code 1 on any problem."""
import glob, os, re, sys

docs = sys.argv[1] if len(sys.argv) > 1 else "Docs/Game/NPCs"
GROUPS = [
    ("LT", "Leitsätze und Produktanforderungen", "02"),
    ("ID", "Identität, Persistenzstufen, Datensatztiefe", "03"),
    ("PS", "Persönlichkeit, Zustände, Zeitskalen", "04"),
    ("GE", "Gedächtnis, Erinnerung, Vergessen", "05"),
    ("WA", "Wahrnehmung, Wissen, Glaube", "06"),
    ("IN", "Informationsfluss, Gerüchte, Kommunikation", "07"),
    ("BE", "Beziehungen: Modell und Wirkung", "08"),
    ("BV", "Beziehungsentwicklung", "09"),
    ("FA", "Familien, Haushalte, Netzwerke", "10"),
    ("NE", "Nemesis: Modell und Datensatz", "11"),
    ("NL", "Nemesis: Lebenszyklus", "12"),
    ("LE", "Autonomes Leben", "13"),
    ("LB", "Lebensereignisse, Alterung, Tod", "14"),
    ("WE", "Weltereignisse und Übernatürliches", "15"),
    ("FR", "Fraktionen: Modell, Mitgliedschaft, Hierarchie", "16"),
    ("TE", "Fraktionen: Territorien, Konflikte, Macht", "17"),
    ("BH", "Behörden, Hunter-Organisationen, Wesen", "18"),
    ("CO", "Begleiter, Crew, Rekrutierung", "19"),
    ("DI", "Dialog und Sichtbarkeit", "20"),
    ("PE", "Weltpersistenz und Speicherung", "21"),
    ("SI", "Simulationsstufen und Skalierung", "22"),
    ("KO", "Koop, Host-Speicherstand, Mods", "23"),
    ("KS", "Konsistenz zwischen Systemen", "24"),
    ("EN", "Entwurfsregeln (unverbindliche Vorschläge)", ""),
]
known = {g[0] for g in GROUPS}
rows = {}      # id -> (cells, module)
problems = []
modfiles = sorted(f for f in glob.glob(os.path.join(docs, "[0-9][0-9]-*.md"))
                  if not os.path.basename(f).startswith(("01-", "26-", "27-", "28-", "29-")))
row_re = re.compile(r"^\| (N-([A-Z]{2})-(\d{3})) \| (.*) \|\s*$")
for f in modfiles:
    mod = os.path.basename(f)[:2]
    in_code = False
    for ln, line in enumerate(open(f, encoding="utf8"), 1):
        if line.startswith("```"):
            in_code = not in_code
        if in_code:
            continue
        m = row_re.match(line)
        if not m:
            continue
        rid, grp = m.group(1), m.group(2)
        cells = [c.strip() for c in m.group(4).split(" | ")]
        want = 2 if grp == "EN" else 3
        if len(cells) != want:
            problems.append(f"{os.path.basename(f)}:{ln}: {rid} has {len(cells)} cells, expected {want}")
            continue
        if grp not in known:
            problems.append(f"{os.path.basename(f)}:{ln}: unknown group {grp}")
        if rid in rows:
            problems.append(f"{os.path.basename(f)}:{ln}: duplicate {rid} (first in module {rows[rid][1]})")
            continue
        rows[rid] = (cells, mod, grp)

out = []
out.append("# NPCs 26: Entscheidungsregister\n")
out.append("> [!IMPORTANT]")
out.append("> **Das Register führt jede Aussage mit Status und Quelle.** Es ist kein Kanon: Nur 📋 ist vom Entwickler bestätigt oder im Master als akzeptiert geführt; 🧭, 💡, ❓ sind es nicht. Eine Zeile beschreibt eine Aussage, keine Umsetzung. Statuszeichen: [Legende](README.md#statuslegende).")
out.append("> **Dieses Dokument ist erzeugt** (`Tools/world-audit/render_npc_register.py`) aus den Tabellen der Module 02 bis 24. Änderungen gehören in die Module, nicht hierher.\n")
out.append("Teil der NPC-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Kürzel in den Quellen: `P<n>` und ‚Frage n‘ meinen die nummerierte Frage in der A-Nachricht, die Antwort steht in der E-Nachricht.\n")
out.append("**Inhalt**\n")
for i, (g, title, mod) in enumerate(GROUPS, 1):
    out.append(f"{i}. [{g}: {title}](#{g.lower()}-{re.sub('[^a-z0-9äöüß _-]', '', title.lower()).replace(' ', '-')})")
out.append("\n---\n")
count = {}
for g, title, mod in GROUPS:
    items = sorted((rid, v) for rid, v in rows.items() if v[2] == g)
    count[g] = len(items)
    anchor = f"{g}: {title}"
    out.append(f"## {anchor}\n")
    if mod:
        out.append(f"Gruppe zu [Modul {mod}]({[os.path.basename(f) for f in modfiles if os.path.basename(f).startswith(mod)][0] if any(os.path.basename(f).startswith(mod) for f in modfiles) else ''}).\n")
    if g == "EN":
        out.append("| ID | Regel | Modul | Bezug |\n|---|---|:---:|---|")
        for rid, (cells, m, _) in items:
            out.append(f"| {rid} | {cells[0]} | {m} | {cells[1]} |")
    else:
        out.append("| ID | Aussage | Status | Quelle |\n|---|---|:---:|---|")
        for rid, (cells, m, _) in items:
            out.append(f"| {rid} | {cells[0]} | {cells[1]} | {cells[2]} |")
    out.append("\n---\n")

# referenced IDs in all docs
ref_re = re.compile(r"\bN-[A-Z]{2}-\d{3}\b")
missing = []
for f in sorted(glob.glob(os.path.join(docs, "*.md"))):
    if os.path.basename(f).startswith("26-"):
        continue
    for ln, line in enumerate(open(f, encoding="utf8"), 1):
        for r in ref_re.findall(line):
            if r not in rows:
                missing.append(f"{os.path.basename(f)}:{ln}: {r}")
problems += [f"reference to undefined ID: {m}" for m in missing]

out.append("## Zählung\n")
out.append("| Gruppe | Anzahl |\n|---|---:|")
for g, title, mod in GROUPS:
    out.append(f"| {g} | {count[g]} |")
out.append(f"| **gesamt** | **{sum(count.values())}** |")
open(os.path.join(docs, "26-Entscheidungsregister.md"), "w", encoding="utf8").write("\n".join(out) + "\n")
print("register entries", len(rows), "groups", {k: v for k, v in count.items() if v})
for p in problems:
    print("PROBLEM", p)

# ---- generated blocks in module 28 (line counts and statistics) ----
def fill(text, begin, end, content):
    i, j = text.index(begin), text.index(end) + len(end)
    return text[:i] + begin + "\n" + content + "\n" + end + text[j:]

cov = os.path.join(docs, "28-Abdeckungsprotokoll.md")
if os.path.exists(cov):
    def linecount(path):
        return open(path, encoding="utf8").read().count("\n")
    for _ in range(3):  # module 28 changes its own size; iterate until stable
        files = sorted(glob.glob(os.path.join(docs, "*.md")))
        lines = ["| Datei | Zeilen |", "|---|---:|"]
        total = 0
        for f in files:
            n = linecount(f); total += n
            lines.append(f"| `{os.path.basename(f)}` | {n} |")
        lines.append(f"| **gesamt ({len(files)} Dateien)** | **{total}** |")
        text = open(cov, encoding="utf8").read()
        new = fill(text, "<!-- BEGIN GENERATED LINE COUNTS -->", "<!-- END GENERATED LINE COUNTS -->", "\n".join(lines))
        if new == text:
            break
        open(cov, "w", encoding="utf8").write(new)
    marks = ["📋", "🧭", "💡", "🔭", "❓", "⚠️"]
    dec = [(rid, v) for rid, v in rows.items() if v[2] != "EN"]
    st = {m: sum(1 for rid, v in dec if m in v[0][1]) for m in marks}
    c27 = open(os.path.join(docs, "27-Konflikte-und-offene-Fragen.md"), encoding="utf8").read() if os.path.exists(os.path.join(docs, "27-Konflikte-und-offene-Fragen.md")) else ""
    cn = len(set(re.findall(r"^\| (C-N\d+) \|", c27, flags=re.M)))
    un = len(set(re.findall(r"^\| (U-N\d+) \|", c27, flags=re.M)))
    qn = len(set(re.findall(r"^\| (Q-N\d+) \|", c27, flags=re.M)))
    def block_lines(path, begin, end):
        t = open(path, encoding="utf8").read()
        return t[t.index(begin):t.index(end) + len(end)].count("\n") + 1 if begin in t else 0
    all_lines = sum(linecount(f) for f in glob.glob(os.path.join(docs, "*.md")))
    gen = linecount(os.path.join(docs, "26-Entscheidungsregister.md"))
    t29 = os.path.join(docs, "29-Testkatalog-und-Abnahmekriterien.md")
    if os.path.exists(t29):
        gen += block_lines(t29, "<!-- BEGIN GENERATED RULE TESTS -->", "<!-- END GENERATED RULE TESTS -->")
    gen += block_lines(cov, "<!-- BEGIN GENERATED LINE COUNTS -->", "<!-- END GENERATED LINE COUNTS -->")
    gen += block_lines(cov, "<!-- BEGIN GENERATED STATS -->", "<!-- END GENERATED STATS -->")
    stats = ["| Kennzahl | Wert |", "|---|---:|",
             f"| Zeilen gesamt (alle Dateien des Ordners) | {all_lines} |",
             f"| davon aus den Modulen erzeugt (Register, Regeltests, Zählblöcke) | {gen} |",
             f"| Rest: von Hand geschriebene Zeilen (Zielgröße 10.000 bis 15.000 gilt für substanzielle Zeilen) | {all_lines - gen} |",
             f"| Register-Einträge gesamt (Aussagen und Entwurfsregeln) | {len(rows)} |",
             f"| davon Aussagen (Gruppen LT bis KS) | {len(dec)} |",
             f"| davon Entwurfsregeln (Gruppe EN) | {len(rows) - len(dec)} |"]
    for m in marks:
        stats.append(f"| Aussagen mit Status {m} (Mehrfachstatus zählt mehrfach) | {st[m]} |")
    stats += [f"| Konflikte C-N | {cn} |", f"| Unklare Antworten U-N | {un} |", f"| Offene Fragen Q-N | {qn} |"]
    text = open(cov, encoding="utf8").read()
    open(cov, "w", encoding="utf8").write(fill(text, "<!-- BEGIN GENERATED STATS -->", "<!-- END GENERATED STATS -->", "\n".join(stats)))
sys.exit(1 if problems else 0)
