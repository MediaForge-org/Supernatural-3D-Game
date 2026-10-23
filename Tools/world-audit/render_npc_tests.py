#!/usr/bin/env python3
"""Fills the generated block in Docs/Game/NPCs/29-Testkatalog-und-Abnahmekriterien.md
from the design-rule tables (N-EN-nnn rows) of the NPC modules.
Usage: python3 render_npc_tests.py [Docs/Game/NPCs]
Only the Python standard library. The text outside the markers is never touched.
Layer heuristic: C = portable core (default), U = Unreal/UI/visible behaviour, I = content/review."""
import glob, os, re, sys

docs = sys.argv[1] if len(sys.argv) > 1 else "Docs/Game/NPCs"
target = os.path.join(docs, "29-Testkatalog-und-Abnahmekriterien.md")
BEGIN, END = "<!-- BEGIN GENERATED RULE TESTS -->", "<!-- END GENERATED RULE TESTS -->"
TITLES = {
    "03": "Identität, Persistenzstufen, Datensatztiefe", "04": "Persönlichkeit, Zustände, Zeitskalen",
    "05": "Gedächtnis, Erinnerung, Vergessen", "06": "Wahrnehmung, Wissen, Glaube",
    "07": "Informationsfluss, Gerüchte, Kommunikation", "08": "Beziehungen: Modell und Wirkung",
    "09": "Beziehungsentwicklung", "10": "Familien, Haushalte, Netzwerke",
    "11": "Nemesis: Modell und Datensatz", "12": "Nemesis: Lebenszyklus",
    "13": "Autonomes Leben", "14": "Lebensereignisse, Alterung, Tod",
    "15": "Reaktion auf Weltereignisse", "16": "Fraktionen: Modell, Mitgliedschaft, Hierarchie",
    "17": "Fraktionen: Territorien, Konflikte, Macht", "18": "Behörden, Hunter-Organisationen, Wesen",
    "19": "Begleiter, Crew, Rekrutierung", "20": "Dialog und Sichtbarkeit",
    "21": "Weltpersistenz und Speicher", "22": "Simulationsstufen und Skalierung",
    "23": "Koop, Host-Speicherstand, Mods", "24": "Integration und Konsistenz",
}
U_PAT = re.compile(r"Spieler-Oberfläche|sichtbar|Karte|Marker|Animation|Materialisier|Actor|Zeile|Dialog|Inspektor|wahrnehmbar|Straße|belebt", re.I)
I_PAT = re.compile(r"Hauptmission|Hauptstory|Kinder|Adult|Mod-|Tattoo|Level|Rekrut", re.I)
row_re = re.compile(r"^\| (N-EN-\d{3}) \| (.*) \| (.*) \|\s*$")
sections = []
total = 0
for f in sorted(glob.glob(os.path.join(docs, "[0-9][0-9]-*.md"))):
    mod = os.path.basename(f)[:2]
    if mod not in TITLES:
        continue
    rows = []
    in_code = False
    for line in open(f, encoding="utf8"):
        if line.startswith("```"):
            in_code = not in_code
        if in_code:
            continue
        m = row_re.match(line)
        if m:
            rows.append((m.group(1), m.group(2).strip(), m.group(3).strip()))
    if not rows:
        continue
    lines = [f"### T-{mod}: {TITLES[mod]}", "",
             f"Gruppe zu [Modul {mod}]({os.path.basename(f)}).", "",
             "| Nr. | Test (Eigenschaft, die gelten muss) | Ebene | Herkunft |", "|---|---|:---:|---|"]
    for i, (rid, rule, bezug) in enumerate(rows, 1):
        layers = ["C"]
        if U_PAT.search(rule):
            layers.append("U")
        if I_PAT.search(rule):
            layers.append("I")
        lines.append(f"| T-{mod}-{i:02d} | {rule} | {', '.join(layers)} | {rid}; {bezug} |")
        total += 1
    sections.append("\n".join(lines))

summary = ["", f"**Insgesamt {total} Regeltests** aus {len(sections)} Modulen.", ""]
block = BEGIN + "\n\n" + "\n\n".join(sections) + "\n" + "\n".join(summary) + "\n" + END
text = open(target, encoding="utf8").read()
i, j = text.index(BEGIN), text.index(END) + len(END)
open(target, "w", encoding="utf8").write(text[:i] + block + text[j:])
print("rule tests", total, "in", len(sections), "modules")
