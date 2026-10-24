#!/usr/bin/env python3
"""Renders Docs/Game/Kampfsystem/27-Entscheidungsregister.md from the ID tables of the combat modules.
Usage: python3 render_combat_register.py [Docs/Game/Kampfsystem]
Only the Python standard library. Rows are read, never written back to the modules.

A decision row in a module looks like:
  | COMBAT-DEC-XX-nnn | statement | status | source | systems | consequence or open point |
The generator checks: unique IDs, cell counts, known group codes, a recognised status word, that every
referenced COMBAT-DEC-XX-nnn exists, that every CQ-nn / CK-nn / CU-nn / CS-nn reference is defined in
module 28 or 26, and that test IDs CT-XX-nnn in module 30 are unique. It also fills the generated
blocks of module 29 (line counts, statistics). Exit code 1 on any problem."""
import glob, os, re, sys

docs = sys.argv[1] if len(sys.argv) > 1 else "Docs/Game/Kampfsystem"
GROUPS = [
    ("GP", "Grundprinzipien", "02"), ("ZA", "Zustände und Architektur", "03"), ("NK", "Nahkampf und Kampfstile", "04"),
    ("FK", "Fernkampf und Schusswaffen", "05"), ("ST", "Steuerung und Eingabekontext", "06"),
    ("DF", "Ausweichen, Blocken, Parieren, Konter", "07"), ("BT", "Kampfbewegung und Traversal", "08"),
    ("WA", "Waffenklassen und Ausrüstung", "09"), ("MU", "Munition, Nachladen, Waffenzustand", "10"),
    ("AU", "Ausdauer, Erschöpfung, Belastung", "11"), ("TK", "Treffermodell und Körperphysik", "12"),
    ("VL", "Verletzungen, Schmerz, Kampffolgen", "13"), ("BW", "Bewusstlosigkeit, Tod, Rettung", "14"),
    ("KS", "Kreaturenkampf und Spezialfähigkeiten", "15"), ("SJ", "Schwächen, Vorbereitung, Jagdmethoden", "16"),
    ("FF", "Fallen, Fesselung, Lebendfang", "17"), ("KI", "Gegner-KI und Kampftaktik", "18"),
    ("NM", "Nemesis, Erinnerung, persistente Kampffolgen", "19"), ("PO", "Fraktionen, Polizei, Kampfkonsequenzen", "20"),
    ("DE", "Kampfvermeidung, Flucht, Deeskalation", "21"), ("AN", "Animationen, Trefferreaktionen, Ragdoll", "22"),
    ("KO", "Koop, Synchronisierung, Autorität", "23"), ("SP", "Speicherstände und Weltpersistenz", "24"),
    ("PF", "Performance, Skalierung, Simulationsstufen", "25"),
]
known = {g[0] for g in GROUPS}
STATUS = [("BESCHLOSSEN", "📋"), ("RICHTUNG", "🧭"), ("ABGELEITET", "🧩"), ("VORSCHLAG", "💡"),
          ("OFFEN", "❓"), ("KONFLIKT", "⚠️"), ("UNBELEGT", "🔍")]
rows, problems = {}, []
modfiles = sorted(f for f in glob.glob(os.path.join(docs, "[0-9][0-9]-*.md"))
                  if not os.path.basename(f).startswith(("01-", "27-", "28-", "29-", "30-")))
row_re = re.compile(r"^\| (COMBAT-DEC-([A-Z]{2})-(\d{3})) \| (.*) \|\s*$")
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
        if len(cells) != 5:
            problems.append(f"{os.path.basename(f)}:{ln}: {rid} has {len(cells)} cells, expected 5")
            continue
        if grp not in known:
            problems.append(f"{os.path.basename(f)}:{ln}: unknown group {grp}")
        if rid in rows:
            problems.append(f"{os.path.basename(f)}:{ln}: duplicate {rid} (first in module {rows[rid][1]})")
            continue
        if not any(w in cells[1] for w, _ in STATUS):
            problems.append(f"{os.path.basename(f)}:{ln}: {rid} has no recognised status word: {cells[1]}")
        want = [g[2] for g in GROUPS if g[0] == grp]
        if want and want[0] != mod:
            problems.append(f"{os.path.basename(f)}:{ln}: {rid} sits in module {mod}, group belongs to {want[0]}")
        rows[rid] = (cells, mod, grp)

all_docs = sorted(glob.glob(os.path.join(docs, "*.md")))
def read(name):
    p = os.path.join(docs, name)
    return open(p, encoding="utf8").read() if os.path.exists(p) else ""
m28, m26, m30 = read("28-Konflikte-und-offene-Fragen.md"), read("26-Querschnittsszenarien.md"), read("30-Testkatalog-und-Abnahmekriterien.md")

out = ["# Kampfsystem 27: Entscheidungsregister\n", "> [!IMPORTANT]",
       "> **Das Register führt jede Aussage mit Status und Quelle.** Es ist kein Kanon: Nur 📋 BESCHLOSSEN ist vom Entwickler festgelegt oder im Master ohne Vorbehalt geführt; 🧭, 🧩, 💡, ❓, ⚠️ und 🔍 sind es nicht. Eine Zeile beschreibt eine Aussage, keine Umsetzung. Statuswörter: [Legende](README.md#statuslegende).",
       "> **Dieses Dokument ist erzeugt** (`Tools/world-audit/render_combat_register.py`) aus den Tabellen der Module 02 bis 25. Änderungen gehören in die Module, nicht hierher.\n",
       "Teil der Kampfsystem-Dokumentation ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Konflikte und Fragen: [Modul 28](28-Konflikte-und-offene-Fragen.md).\n",
       "**Inhalt**\n"]
for i, (g, title, mod) in enumerate(GROUPS, 1):
    out.append(f"{i}. [{g}: {title}](#{g.lower()}-{re.sub('[^a-z0-9äöüß _-]', '', title.lower()).replace(' ', '-')})")
out.append("\n---\n")
count = {}
for g, title, mod in GROUPS:
    items = sorted((rid, v) for rid, v in rows.items() if v[2] == g)
    count[g] = len(items)
    out.append(f"## {g}: {title}\n")
    target = [os.path.basename(f) for f in modfiles if os.path.basename(f).startswith(mod)]
    out.append(f"Gruppe zu [Modul {mod}]({target[0] if target else ''}).\n")
    out.append("| ID | Aussage | Status | Quelle | Systeme | Folge oder offener Punkt |\n|---|---|:---:|---|---|---|")
    for rid, (c, _, _) in items:
        out.append(f"| {rid} | {c[0]} | {c[1]} | {c[2]} | {c[3]} | {c[4]} |")
    out.append("\n---\n")

# references
ref_dec = re.compile(r"\bCOMBAT-DEC-[A-Z]{2}-\d{3}\b")
defined_cq = set(re.findall(r"^\| `?(CQ-\d+)`? \|", m28, flags=re.M))
defined_ck = set(re.findall(r"^\| `?(CK-\d+)`? \|", m28, flags=re.M))
defined_cu = set(re.findall(r"^\| `?(CU-\d+)`? \|", m28, flags=re.M))
defined_cs = set(re.findall(r"^## (CS-\d+)", m26, flags=re.M))
used_cq, used_ck, used_cu, used_cs = set(), set(), set(), set()
for f in all_docs:
    base = os.path.basename(f)
    if base.startswith("27-"):
        continue
    for ln, line in enumerate(open(f, encoding="utf8"), 1):
        for r in ref_dec.findall(line):
            if r not in rows:
                problems.append(f"{base}:{ln}: reference to undefined ID {r}")
        if not base.startswith("28-"):
            used_cq.update(re.findall(r"\bCQ-\d+\b", line)); used_ck.update(re.findall(r"\bCK-\d+\b", line)); used_cu.update(re.findall(r"\bCU-\d+\b", line))
        used_cs.update(re.findall(r"\bCS-\d+\b", line))
for tag, used, defined in (("CQ", used_cq, defined_cq), ("CK", used_ck, defined_ck), ("CU", used_cu, defined_cu), ("CS", used_cs, defined_cs)):
    for u in sorted(used - defined):
        problems.append(f"{tag}: {u} is referenced but not defined")
unused_cq = sorted(defined_cq - used_cq)
test_ids = re.findall(r"^\| (CT-[A-Z]{2}-\d{3}) \|", m30, flags=re.M)
for t in sorted({t for t in test_ids if test_ids.count(t) > 1}):
    problems.append(f"duplicate test ID {t}")
dec_in_tests = set(ref_dec.findall(m30))

out.append("## Zählung\n")
out.append("| Gruppe | Anzahl |\n|---|---:|")
for g, title, mod in GROUPS:
    out.append(f"| {g} | {count[g]} |")
out.append(f"| **gesamt** | **{sum(count.values())}** |")
open(os.path.join(docs, "27-Entscheidungsregister.md"), "w", encoding="utf8").write("\n".join(out) + "\n")
print("register entries", len(rows), {k: v for k, v in count.items() if v})
print("CQ defined", len(defined_cq), "CK", len(defined_ck), "CU", len(defined_cu), "CS", len(defined_cs), "tests", len(test_ids))
if unused_cq:
    print("INFO: CQ defined in module 28 but referenced in no module:", ", ".join(unused_cq))
for p in problems:
    print("PROBLEM", p)

def fill(text, begin, end, content):
    i, j = text.index(begin), text.index(end) + len(end)
    return text[:i] + begin + "\n" + content + "\n" + end + text[j:]

def linecount(path):
    return open(path, encoding="utf8").read().count("\n")

cov = os.path.join(docs, "29-Abdeckungsprotokoll.md")
if os.path.exists(cov):
    for _ in range(3):
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
    st = {w: sum(1 for v in rows.values() if w in v[0][1]) for w, _ in STATUS}
    stats = ["| Kennzahl | Wert |", "|---|---:|", f"| Register-Einträge (Anforderungen und Aussagen) | {len(rows)} |"]
    for w, e in STATUS:
        stats.append(f"| Einträge mit Status {e} {w} | {st[w]} |")
    stats += [f"| Konflikte CK | {len(defined_ck)} |", f"| Unbelegte oder unklare Angaben CU | {len(defined_cu)} |",
              f"| Offene Fragen CQ | {len(defined_cq)} |", f"| Szenarien CS | {len(defined_cs)} |",
              f"| Testfälle CT | {len(test_ids)} |",
              f"| Register-IDs, die ein Testfall referenziert | {len(dec_in_tests & set(rows))} von {len(rows)} |"]
    text = open(cov, encoding="utf8").read()
    open(cov, "w", encoding="utf8").write(fill(text, "<!-- BEGIN GENERATED STATS -->", "<!-- END GENERATED STATS -->", "\n".join(stats)))
sys.exit(1 if problems else 0)
