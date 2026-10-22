#!/usr/bin/env python3
"""Erzeugt Modul 25 (Weltaussagen-Index) aus Register (Modul 20), Konflikten (21), Fragen (22) und den Modultexten.
Usage: python3 render_index.py <Docs/Game/World> <Ausgabedatei>   (die Ausgabe wird erst am Ende geschrieben, weil Modul 25 sich selbst in der Modulliste liest)
Reine Auswertung vorhandener Dokumente; keine neuen Aussagen."""
import re, sys, os, glob, collections
d = sys.argv[1]
import io, contextlib
_buf = io.StringIO(); _ctx = contextlib.redirect_stdout(_buf); _ctx.__enter__()
mods = {os.path.basename(f)[:2]: os.path.basename(f) for f in sorted(glob.glob(d + "/[0-9][0-9]-*.md"))}
def text(n): return open(os.path.join(d, mods[n]), encoding="utf-8").read()
title = {n: open(os.path.join(d, f), encoding="utf-8").readline().strip("# \n") for n, f in mods.items()}
rows = []
for line in text("20").splitlines():
    m = re.match(r"\| (W-([A-Z]+)-(\d+)) \| (.*?) \| (.*?) \| (.*?) \| (.*?) ?\|", line)
    if m: rows.append(m.groups())
area = {"GE": ("Weltgröße und Grundstruktur", "02, 03"), "KA": ("Karte und Abnahme", "17, 19"),
        "ST": ("Städte und Siedlungen", "06, 07"), "UR": ("Urbane Gestaltung", "06, 08"),
        "NA": ("Natur, Berge, Wald", "04"), "WA": ("Gewässer", "05"), "VK": ("Straßen, Schiene", "09"),
        "UG": ("Untergrund: Lore und Raum", "10, 11"), "GS": ("Untergrund: Gesellschaft", "12"),
        "WI": ("Wirtschaft Oberfläche", "12"), "FZ": ("Fahrzeuge", "13"), "FP": ("Fahrphysik", "14"),
        "MO": ("Motorsport", "15"), "TV": ("NPC-Verkehr", "16"), "TE": ("Technik, Weltgenerierung", "17, 18"),
        "RE": ("Reiche, Zeit", "26"), "AK": ("Aktivitäten", "08")}
def refs(modn, pat):
    out = collections.defaultdict(set)
    for line in text(modn).splitlines():
        ids = set(re.findall(r"W-[A-Z]+-\d+", line))
        k = re.findall(pat, line)
        for i in ids:
            for x in k: out[i].add(x)
    return out
kon = refs("21", r"\bK\d+\b"); of = refs("22", r"\bOF-\d+\b")
for line in text("20").splitlines():  # K-Verweise stehen vor allem in den Anmerkungen des Registers
    m = re.match(r"\| (W-[A-Z]+-\d+) \|", line)
    if m:
        for k in re.findall(r"\bK\d+\b", line): kon[m.group(1)].add(k)
P = print
P("# Welt 25: Weltaussagen-Index\n")
P("> [!IMPORTANT]\n> **Generiertes Modul** (`Tools/world-audit/render_index.py`). Es enthält keine eigenen Aussagen, sondern ordnet das [Register](20-Entscheidungsregister.md), die [Konflikte](21-Konflikte-und-Widersprueche.md) und die [offenen Fragen](22-Offene-Fragen-Katalog.md) nach Thema, Status und Modul. Statuszeichen: [Legende](README.md#statuslegende). Bei Abweichungen gilt das Register.\n")
P("Teil der Welt-Dokumentation ([Index](README.md)).\n")
P("**Inhalt**\n\n1. [Themenübersicht](#1-themenübersicht)\n2. [Einträge nach Status](#2-einträge-nach-status)\n3. [Einträge mit Konflikt oder offener Frage](#3-einträge-mit-konflikt-oder-offener-frage)\n4. [Begriffsindex](#4-begriffsindex)\n5. [Module im Überblick](#5-module-im-überblick)\n\n---\n")
P("## 1. Themenübersicht\n")
P("| Kürzel | Thema | Einträge | davon 📋 | Module | Konfliktverweise |\n|---|---|---:|---:|---|---|")
for k, (name, m) in area.items():
    r = [x for x in rows if x[1] == k]
    ks = sorted({c for x in r for c in kon.get(x[0], ())}, key=lambda s: int(s[1:]))
    mm = ", ".join(f"[{x.strip()}]({mods[x.strip()]})" for x in m.split(",") if x.strip() in mods)
    P(f"| {k} | {name} | {len(r)} | {sum('📋' in x[4] for x in r)} | {mm} | {', '.join(ks) or '–'} |")
P(f"\nSumme: {len(rows)} Einträge.\n")
P("---\n\n## 2. Einträge nach Status\n")
P("Mischstatus (zum Beispiel „🕰️ / 📋“) erscheinen unter jedem enthaltenen Zeichen.\n")
stat = [("📋", "bestätigt"), ("🧭", "vorläufig"), ("💡", "Vorschlag"), ("🔭", "Vision"), ("❓", "offen"), ("🕰️", "historisch ersetzt"), ("⚠️", "widersprüchlich")]
for s, nm in stat:
    r = [x for x in rows if s in x[4]]
    P(f"### {s} {nm} ({len(r)})\n")
    P("; ".join(f"{x[0]}" for x in r) or "–")
    P("")
P("---\n\n## 3. Einträge mit Konflikt oder offener Frage\n")
P("Nur Einträge, deren ID in Modul 21 oder 22 ausdrücklich genannt wird.\n")
P("| ID | Status | Konflikte | Fragen | Aussage (gekürzt) |\n|---|:---:|---|---|---|")
for x in rows:
    a, b = sorted(kon.get(x[0], ()), key=lambda s: int(s[1:])), sorted(of.get(x[0], ()))
    if a or b:
        t = re.sub(r"\s+", " ", x[3]).strip()
        t = (t[:110].rsplit(" ", 1)[0] + " …") if len(t) > 112 else t
        P(f"| {x[0]} | {x[4]} | {', '.join(a) or '–'} | {', '.join(b) or '–'} | {t.replace('|', '/')} |")
P("\n---\n\n## 4. Begriffsindex\n")
P("Trefferzahl je Modul (Zeilen, die den Begriff enthalten, ohne Module 20 bis 25, damit Register und Index nicht doppelt zählen). Ein Treffer ist ein Fundhinweis, **keine** Aussage über den Status.\n")
terms = ["WORLD_700", "WORLD_1500", "2.200", "110 km²", "Ostinsel", "Metropolis", "Glenhaven", "Ashford", "Greywater", "Alder", "Meridian", "Bunker", "Tesla", "Schlüssel", "Elite", "Hölle", "Zeitreise", "Flughafen", "Motorsport", "Eisenbahn|Schiene", "World Partition", "GeoPackage", "Gipfel", "Hunter"]
body = {n: text(n).splitlines() for n in mods if n not in ("20", "21", "22", "23", "24", "25")}
hdr = sorted(body)
P("| Begriff | " + " | ".join(hdr) + " |\n|---|" + "---:|" * len(hdr))
for t in terms:
    rx = re.compile(t)
    cells = [str(sum(1 for l in body[n] if rx.search(l))) or "0" for n in hdr]
    P(f"| {t.replace('|', ' oder ')} | " + " | ".join(c if c != "0" else "·" for c in cells) + " |")
P("\n---\n\n## 5. Module im Überblick\n")
P("| Nr. | Titel | Zeilen |\n|---|---|---:|")
for n, f in mods.items():
    P(f"| {n} | [{title[n]}]({f}) | {len(open(os.path.join(d, f), encoding='utf-8').read().splitlines())} |")

_ctx.__exit__(None, None, None)
open(sys.argv[2], "w", encoding="utf-8").write(_buf.getvalue())
