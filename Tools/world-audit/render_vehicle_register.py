#!/usr/bin/env python3
"""Renders Docs/Game/Fahrzeuge-und-Verkehr/28-Entscheidungsregister.md from the ID tables of the vehicle modules.
Usage: python3 render_vehicle_register.py [Docs/Game/Fahrzeuge-und-Verkehr]
Only the Python standard library. Rows are read, never written back to the modules.

A decision row in a module looks like:
  | VEH-DEC-XX-nnn | statement | status | source | systems | consequence or open point |
Checks: unique IDs, cell counts, known group codes, a recognised status word, module/group mapping, that every
referenced VEH-DEC-XX-nnn exists, that every CQ-nn / CK-nn / CU-nn reference is defined in module 29, that test IDs
CT-XX-nnn in module 31 are unique, and that references into the sibling registers (W-, N-, E-, K-, COMBAT-DEC-)
resolve to a row of that register. Also fills the generated blocks of module 30 (line counts, statistics).
Exit code 1 on any problem."""
import glob, os, re, sys

docs = sys.argv[1] if len(sys.argv) > 1 else "Docs/Game/Fahrzeuge-und-Verkehr"
GROUPS = [
    ("GP", "Vision und Grundprinzipien", "02"), ("KL", "Fahrzeugklassen und Fahrzeugdaten", "03"),
    ("PH", "Fahrphysik und Fahrverhalten", "04"), ("ST", "Steuerung, Kamera und Anzeige", "05"),
    ("EI", "Einsteigen, Aussteigen, Sitze, Kofferraum", "06"), ("ZV", "Zustand, Verschleiß, Schaden", "07"),
    ("WE", "Reparatur, Wartung, Werkstätten", "08"), ("KE", "Kraftstoff, Energie, Tankstellen", "09"),
    ("SN", "Straßennetz und Straßentypen", "10"), ("KR", "Kreuzungen, Signale, Schilder", "11"),
    ("SP", "Stadtverkehr, Parken, Garagen", "12"), ("KI", "Verkehrs-KI und Navigation", "13"),
    ("NF", "NPC-Fahrer und NPC-Fahrzeuge", "14"), ("VD", "Dichte, Stau, Baustellen, Sperrungen", "15"),
    ("UP", "Unfälle, Pannen, Einsatz- und Sonderverkehr", "16"), ("VT", "Fußgänger, Radfahrer, weitere Teilnehmer", "17"),
    ("BZ", "Besitz, Zulassung, Diebstahl", "18"), ("PV", "Polizei und Verkehrsüberwachung", "19"),
    ("IT", "Interaktionen, Transport, Jobs", "20"), ("EJ", "Fahrzeuge in Ermittlungen und Jagden", "21"),
    ("PE", "Persistenz und Lebenszyklen", "22"), ("KO", "Koop und Netzwerk", "23"),
    ("WF", "WorldForge-Schnittstelle", "24"), ("UE", "Unreal-Integration", "25"),
    ("LS", "Simulationsstufen, LOD, Streaming", "26"), ("PF", "Performance und Skalierung", "27"),
]
known = {g[0] for g in GROUPS}
STATUS = [("BESCHLOSSEN", "📋"), ("RICHTUNG", "🧭"), ("ABGELEITET", "🧩"), ("VORSCHLAG", "💡"),
          ("OFFEN", "❓"), ("KONFLIKT", "⚠️"), ("UNBELEGT", "🔍")]
SKIP = ("01-", "28-", "29-", "30-", "31-")
rows, problems = {}, []
modfiles = sorted(f for f in glob.glob(os.path.join(docs, "[0-9][0-9]-*.md"))
                  if not os.path.basename(f).startswith(SKIP))
row_re = re.compile(r"^\| (VEH-DEC-([A-Z]{2})-(\d{3})) \| (.*) \|\s*$")
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
m29, m31 = read("29-Konflikte-und-offene-Fragen.md"), read("31-Testkatalog-Phasen-und-Abnahme.md")

out = ["# Fahrzeuge und Verkehr 28: Entscheidungsregister\n", "> [!IMPORTANT]",
       "> **Das Register führt jede Aussage mit Status und Quelle.** Es ist kein Kanon: Nur 📋 BESCHLOSSEN ist vom Entwickler festgelegt oder im Master ohne Vorbehalt geführt; 🧭, 🧩, 💡, ❓, ⚠️ und 🔍 sind es nicht. Eine Zeile beschreibt eine Aussage, keine Umsetzung. Statuswörter: [Legende](README.md#statuslegende).",
       "> **Dieses Dokument ist erzeugt** (`Tools/world-audit/render_vehicle_register.py`) aus den Tabellen der Module 02 bis 27. Änderungen gehören in die Module, nicht hierher.\n",
       "Teil der Dokumentation Fahrzeuge und Verkehr ([Index](README.md)). Quellenschlüssel: [Modul 01](01-Quellenlage-und-Methodik.md). Konflikte und Fragen: [Modul 29](29-Konflikte-und-offene-Fragen.md).\n",
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

# references inside the vehicle docs
ref_dec = re.compile(r"\bVEH-DEC-[A-Z]{2}-\d{3}\b")
defined = {t: set(re.findall(r"^\| `?(" + t + r"-\d+)`? \|", m29, flags=re.M)) for t in ("CQ", "CK", "CU")}
used = {t: set() for t in defined}
# sibling registers
sib = {"W": ("World/20-Entscheidungsregister.md", r"W-[A-Z]{2}-\d{3}"), "N": ("NPCs/26-Entscheidungsregister.md", r"N-[A-Z]{2}-\d{3}"),
       "E": ("Ermittlung/20-Entscheidungsregister.md", r"E-[A-Z]{2}-\d{3}"), "K": ("Kreaturen/20-Entscheidungsregister.md", r"K-[A-Z]{2}-\d{3}"),
       "C": ("Kampfsystem/27-Entscheidungsregister.md", r"COMBAT-DEC-[A-Z]{2}-\d{3}")}
sib_ids = {}
for k, (rel, pat) in sib.items():
    p = os.path.join(os.path.dirname(docs.rstrip("/")), rel)
    sib_ids[k] = set(re.findall(r"^\| (" + pat + r") \|", open(p, encoding="utf8").read(), flags=re.M)) if os.path.exists(p) else None
    if sib_ids[k] is None:
        problems.append(f"sibling register missing: {rel}")
sib_checked = 0
for f in all_docs:
    base = os.path.basename(f)
    if base.startswith("28-"):
        continue
    for ln, line in enumerate(open(f, encoding="utf8"), 1):
        for r in ref_dec.findall(line):
            if r not in rows:
                problems.append(f"{base}:{ln}: reference to undefined ID {r}")
        if not base.startswith("29-"):
            for t in used:
                used[t].update(re.findall(r"\b" + t + r"-\d+\b", line))
        for k, (rel, pat) in sib.items():
            if sib_ids[k] is None:
                continue
            lead = r"(?<![A-Z-])" if k != "C" else ""
            for r in re.findall(lead + r"\b" + pat + r"\b", line):
                if k == "W" and r.startswith("VEH"):
                    continue
                sib_checked += 1
                if r not in sib_ids[k]:
                    problems.append(f"{base}:{ln}: reference {r} not found in {sib[k][0]}")
for t in defined:
    for u in sorted(used[t] - defined[t]):
        problems.append(f"{t}: {u} is referenced but not defined")
unused_cq = sorted(defined["CQ"] - used["CQ"])
test_ids = re.findall(r"^\| (CT-[A-Z]{2}-\d{3}) \|", m31, flags=re.M)
for t in sorted({t for t in test_ids if test_ids.count(t) > 1}):
    problems.append(f"duplicate test ID {t}")
dec_in_tests = set(ref_dec.findall(m31))

out.append("## Zählung\n")
out.append("| Gruppe | Anzahl |\n|---|---:|")
for g, title, mod in GROUPS:
    out.append(f"| {g} | {count[g]} |")
out.append(f"| **gesamt** | **{sum(count.values())}** |")
open(os.path.join(docs, "28-Entscheidungsregister.md"), "w", encoding="utf8").write("\n".join(out) + "\n")
print("register entries", len(rows), {k: v for k, v in count.items() if v})
print("CQ defined", len(defined["CQ"]), "CK", len(defined["CK"]), "CU", len(defined["CU"]), "tests", len(test_ids), "sibling refs checked", sib_checked)
if unused_cq:
    print("INFO: CQ defined in module 29 but referenced in no module:", ", ".join(unused_cq))
for p in problems:
    print("PROBLEM", p)

def fill(text, begin, end, content):
    i, j = text.index(begin), text.index(end) + len(end)
    return text[:i] + begin + "\n" + content + "\n" + end + text[j:]

cov = os.path.join(docs, "30-Abdeckungsprotokoll.md")
if os.path.exists(cov):
    for _ in range(3):
        files = sorted(glob.glob(os.path.join(docs, "*.md")))
        lines = ["| Datei | Zeilen |", "|---|---:|"]
        total = 0
        for f in files:
            n = open(f, encoding="utf8").read().count("\n"); total += n
            lines.append(f"| `{os.path.basename(f)}` | {n} |")
        lines.append(f"| **gesamt ({len(files)} Dateien)** | **{total}** |")
        text = open(cov, encoding="utf8").read()
        new = fill(text, "<!-- BEGIN GENERATED LINE COUNTS -->", "<!-- END GENERATED LINE COUNTS -->", "\n".join(lines))
        if new == text:
            break
        open(cov, "w", encoding="utf8").write(new)
    st = {w: sum(1 for v in rows.values() if w in v[0][1]) for w, _ in STATUS}
    stats = ["| Kennzahl | Wert |", "|---|---:|", f"| Register-Einträge | {len(rows)} |"]
    for w, e in STATUS:
        stats.append(f"| Einträge mit Status {e} {w} | {st[w]} |")
    stats += [f"| Konflikte CK | {len(defined['CK'])} |", f"| Unbelegte oder unklare Angaben CU | {len(defined['CU'])} |",
              f"| Offene Fragen CQ | {len(defined['CQ'])} |", f"| Testfälle CT | {len(test_ids)} |",
              f"| Register-IDs, die ein Testfall referenziert | {len(dec_in_tests & set(rows))} von {len(rows)} |",
              f"| Geprüfte Verweise in Nachbarregister | {sib_checked} |"]
    text = open(cov, encoding="utf8").read()
    open(cov, "w", encoding="utf8").write(fill(text, "<!-- BEGIN GENERATED STATS -->", "<!-- END GENERATED STATS -->", "\n".join(stats)))
sys.exit(1 if problems else 0)
